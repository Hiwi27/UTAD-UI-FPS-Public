// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "UTAD_UI_FPSCharacter.h"
#include "UTAD_UI_FPSProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Camera/CameraComponent.h"
#include "Components/Image.h"
#include "UI/Crosshair.h"
#include "UI/PlayerHUD.h"
#include "DrawDebugHelpers.h"
#include "UTAD_UI_FPS_Enemy.h"
#include "Components/ProgressBar.h"
#include "UI/AmmoCounter.h"
#include "UI/ReloadBar.h"

#define RELOAD_TIME 1.f

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
	: FireSound(nullptr), FireAnimation(nullptr), FireMappingContext(nullptr), FireAction(nullptr),
	  ReloadAction(nullptr),
	  Character(nullptr), ReloadTimer(0.f)
	  , bIsReloading(false)
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UTP_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bIsReloading)
	{
		ReloadTimer += DeltaTime;
		// To test ReloadTimer
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), ReloadTimer));
		Character->GetPlayerHudInstance()->ReloadBarWidget->UpdateReloadBarValue(ReloadTimer);
	}

	if(Character!= nullptr)
	{
		
	FHitResult OutHit;
	GetWorld()->LineTraceSingleByChannel(OutHit,Character->GetFirstPersonCameraComponent()->GetComponentLocation(), (Character->GetFirstPersonCameraComponent()->GetForwardVector()* 2000) + Character->GetFirstPersonCameraComponent()->GetComponentLocation(),ECC_Visibility );
	DrawDebugLine(GetWorld(),Character->GetFirstPersonCameraComponent()->GetComponentLocation(), (Character->GetFirstPersonCameraComponent()->GetForwardVector()* 2000) + Character->GetFirstPersonCameraComponent()->GetComponentLocation(), FColor::Red);
		if(Cast<AUTAD_UI_FPS_Enemy>(OutHit.GetActor()))
		{
			//Make crosshair red when aiming a enemy
			Character->GetPlayerHudInstance()->CrosshairWidget->Crosshair->SetColorAndOpacity(FLinearColor::Red);
		}
		else
		{
		    //Reset crosshair color
			// Character->GetPlayerHudInstance()->CrosshairWidget->Crosshair->SetBrushTintColor(Character->GetPlayerHudInstance()->CrosshairWidget->OriginalColor);
			Character->GetPlayerHudInstance()->CrosshairWidget->Crosshair->SetColorAndOpacity(FLinearColor::White);
		}
	}

}

void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if (CurrentNumBullets <= 0)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			World->SpawnActor<AUTAD_UI_FPSProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			Character->GetPlayerHudInstance()->CrosshairWidget->FireAnimation = true;

		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}

	--CurrentNumBullets;
	SetCurrentNumBullets(CurrentNumBullets);

}

void UTP_WeaponComponent::StartReload()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	int playerBullets = Character->GetTotalBullets();
	playerBullets += CurrentNumBullets;

	if (playerBullets <= CurrentNumBullets || CurrentNumBullets == MagazineSize)
	{
		return;
	}

	bIsReloading = true;
	ReloadTimer = 0.f;
}

void UTP_WeaponComponent::CompleteReload()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	bIsReloading = false;

	int playerBullets = Character->GetTotalBullets();
	playerBullets += CurrentNumBullets;

	CurrentNumBullets = __min(MagazineSize, playerBullets);

	Character->SetTotalBullets(playerBullets - CurrentNumBullets);
	SetCurrentNumBullets(CurrentNumBullets);
	Character->GetPlayerHudInstance()->ReloadBarWidget->UpdateReloadBarValue(0.0f);
}

void UTP_WeaponComponent::CancelReload()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	bIsReloading = false;

	Character->GetPlayerHudInstance()->ReloadBarWidget->UpdateReloadBarValue(0.0f);
}

int UTP_WeaponComponent::GetMagazineSize()
{
	return MagazineSize;
}

void UTP_WeaponComponent::SetMagazineSize(int NewMagazineSize)
{
	MagazineSize = NewMagazineSize;
}

int UTP_WeaponComponent::GetCurrentNumBullets()
{
	return CurrentNumBullets;
}

void UTP_WeaponComponent::SetCurrentNumBullets(int NewCurrentNumBullets)
{
	CurrentNumBullets = NewCurrentNumBullets;
	Character->GetPlayerHudInstance()->AmmoCounterWidget->UpdateCurrentAmmo(CurrentNumBullets);
}

void UTP_WeaponComponent::AttachWeapon(AUTAD_UI_FPSCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr || Character->GetHasRifle())
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	Character->SetAttachedWeaponComponent(this);
	
	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);
	Character->GetPlayerHudInstance()->ReloadBarWidget->UpdateReloadBarValue(ReloadTimer);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);

			// StartReload
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::StartReload);

			// CompleteReload
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::CompleteReload);

			// CancelReload
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Canceled, this, &UTP_WeaponComponent::CancelReload);
		}
	}
	SetCurrentNumBullets(CurrentNumBullets);
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void UTP_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTP_WeaponComponent::InitializeComponent()
{
	Super::InitializeComponent();


}
