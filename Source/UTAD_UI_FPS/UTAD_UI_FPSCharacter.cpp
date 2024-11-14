// Copyright Epic Games, Inc. All Rights Reserved.

#include "UTAD_UI_FPSCharacter.h"
#include "UTAD_UI_FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// UI
#include "TP_WeaponComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "UI/AbilitySystemHUD.h"
#include "UI/AmmoCounter.h"
#include "UI/PlayerHealthBar.h"
#include "UI/PlayerHUD.h"
#include "UI/ReloadBar.h"
#include "UI/SplashScreen.h"

//////////////////////////////////////////////////////////////////////////
// AUTAD_UI_FPSCharacter

AUTAD_UI_FPSCharacter::AUTAD_UI_FPSCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AUTAD_UI_FPSCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Setup Player HUD
	if (PlayerHUDWidget)
	{
		PlayerHUDInstance = CreateWidget<UPlayerHUD>(GetWorld(), PlayerHUDWidget);
		PlayerHUDInstance->AddToViewport();
		PlayerHUDInstance->ShowNoWeapon();
		PlayerHUDInstance->PlayerHealthBarWidget->PlayerHealthBar->SetPercent(Health/100.0f);
		UTP_WeaponComponent* prueba = Cast<UTP_WeaponComponent>( GetComponentByClass(UTP_WeaponComponent::StaticClass()));
		if(prueba)
		{
			PlayerHUDInstance->AmmoCounterWidget->UpdateCurrentAmmo(prueba->CurrentNumBullets);
		}
		PlayerHUDInstance->AmmoCounterWidget->UpdateTotalAmmo(TotalBullets);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player HUD Widget not assigned to UTAD_UI_FPSCharacter"));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player HUD Widget not assigned to UTAD_UI_FPSCharacter"));
	}

	if (AbilitySystemHUDWidget)
	{
		AblilitySystyemHUDInstance = CreateWidget<UAbilitySystemHUD>(GetWorld(), AbilitySystemHUDWidget);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AbilitySystem HUD Widget not assigned to UTAD_UI_FPSCharacter"));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("AbilitySystem HUD Widget not assigned to UTAD_UI_FPSCharacter"));
	}

	if(GetHasRifle())
	{
		PlayerHUDInstance->ShowAll();
	}
	else
	{
		PlayerHUDInstance->Hide();
	}

	GetPlayerHudInstance()->SplashScreenWidget->Show();
	
}

//////////////////////////////////////////////////////////////////////////// Input

void AUTAD_UI_FPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUTAD_UI_FPSCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUTAD_UI_FPSCharacter::Look);

		//AbilitySystemMenu
		EnhancedInputComponent->BindAction(ToggleAblilityHUD, ETriggerEvent::Completed, this, &AUTAD_UI_FPSCharacter::ToggleAblityHUD);

		//AbilitySystemMenu Navigation
		EnhancedInputComponent->BindAction(AblilityHUD_UP, ETriggerEvent::Completed, this, &AUTAD_UI_FPSCharacter::AbilitySystemHUD_UP);
		EnhancedInputComponent->BindAction(AblilityHUD_RIGHT, ETriggerEvent::Completed, this, &AUTAD_UI_FPSCharacter::AbilitySystemHUD_RIGTH);
		EnhancedInputComponent->BindAction(AblilityHUD_DOWN, ETriggerEvent::Completed, this, &AUTAD_UI_FPSCharacter::AbilitySystemHUD_DOWN);
		EnhancedInputComponent->BindAction(AblilityHUD_LEFT, ETriggerEvent::Completed, this, &AUTAD_UI_FPSCharacter::AbilitySystemHUD_LEFT);
		EnhancedInputComponent->BindAction(AblilityHUD_ENTER, ETriggerEvent::Completed, this, &AUTAD_UI_FPSCharacter::AbilitySystemHUD_ENTER);

	}
}


void AUTAD_UI_FPSCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AUTAD_UI_FPSCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AUTAD_UI_FPSCharacter::ToggleAblityHUD(const FInputActionValue& Value)
{
	if(AbilityHUDToogled == false)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(DefaultMappingContext);           // Remove the current mapping context
				Subsystem->AddMappingContext(MenuMappingContext, 1);

				// PlayerHUDInstance->RemoveFromParent();
				AblilitySystyemHUDInstance->AddToViewport();
				AbilityHUDToogled = true;
			}
		}
	}
	else
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(MenuMappingContext);           // Remove the current mapping context
				Subsystem->AddMappingContext(DefaultMappingContext, 1);
				
				AblilitySystyemHUDInstance->RemoveFromParent();
				// PlayerHUDInstance->AddToViewport();
				AbilityHUDToogled = false;
			}
		}
	}
	
}

void AUTAD_UI_FPSCharacter::AbilitySystemHUD_UP(const FInputActionValue& Value)
{
	AblilitySystyemHUDInstance->Up();
}

void AUTAD_UI_FPSCharacter::AbilitySystemHUD_RIGTH(const FInputActionValue& Value)
{
	AblilitySystyemHUDInstance->Right();

}

void AUTAD_UI_FPSCharacter::AbilitySystemHUD_DOWN(const FInputActionValue& Value)
{
	AblilitySystyemHUDInstance->Down();

}

void AUTAD_UI_FPSCharacter::AbilitySystemHUD_LEFT(const FInputActionValue& Value)
{
	AblilitySystyemHUDInstance->Left();

}

void AUTAD_UI_FPSCharacter::AbilitySystemHUD_ENTER(const FInputActionValue& Value)
{
	AblilitySystyemHUDInstance->Enter();

}

void AUTAD_UI_FPSCharacter::SetHealth(int NewHealth)
{
	int ClampedNewHealth = FMath::Clamp(NewHealth, 0, MaxHealth);
	if (ClampedNewHealth != Health)
	{
		Health = ClampedNewHealth;
		PlayerHUDInstance->PlayerHealthBarWidget->PlayerHealthBar->SetPercent(Health/100.0f);
	}
}

int AUTAD_UI_FPSCharacter::GetHealth()
{
	return Health;
}

void AUTAD_UI_FPSCharacter::SetMaxHealth(int NewMaxHealth)
{
	MaxHealth = FMath::Max(0, NewMaxHealth);

}

int AUTAD_UI_FPSCharacter::GetMaxHealth()
{
	return MaxHealth;
}

void AUTAD_UI_FPSCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
	PlayerHUDInstance->ShowAll();
	
}

bool AUTAD_UI_FPSCharacter::GetHasRifle()
{
	return bHasRifle;
}

void AUTAD_UI_FPSCharacter::SetTotalBullets(int NewTotalBullets)
{
	TotalBullets = NewTotalBullets;
	GetPlayerHudInstance()->AmmoCounterWidget->UpdateTotalAmmo(NewTotalBullets);
}

int AUTAD_UI_FPSCharacter::GetTotalBullets()
{
	return TotalBullets;
}

void AUTAD_UI_FPSCharacter::AddBullets(int Bullets)
{
	TotalBullets += Bullets;
}

void AUTAD_UI_FPSCharacter::SetAttachedWeaponComponent(UTP_WeaponComponent* WeaponComponent)
{
	AttachedWeaponComponent = WeaponComponent;
}
