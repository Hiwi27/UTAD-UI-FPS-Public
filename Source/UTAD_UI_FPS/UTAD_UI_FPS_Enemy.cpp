// Fill out your copyright notice in the Description page of Project Settings.


#include "UTAD_UI_FPS_Enemy.h"

#include "Components/ProgressBar.h"
#include "UI/EnemyHealthBar.h"
#include "Components/WidgetComponent.h"

void AUTAD_UI_FPS_Enemy::BeginPlay()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Super::BeginPlay();
	Health = MaxHealth;

	UUserWidget* WidgetInstance = WidgetComponent->GetUserWidgetObject();
	if (WidgetInstance)
	{
		EnemyHealthBarComponent = Cast<UEnemyHealthBar>(WidgetInstance);
		if (EnemyHealthBarComponent)
		{
			EnemyHealthBarComponent->EnemyHealthBar->SetPercent(Health/MaxHealth);
		}
	}


}

AUTAD_UI_FPS_Enemy::AUTAD_UI_FPS_Enemy()
{
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>((TEXT("HB")));
	WidgetComponent->AttachToComponent(GetStaticMeshComponent(),FAttachmentTransformRules::KeepRelativeTransform);
}

void AUTAD_UI_FPS_Enemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AUTAD_UI_FPS_Enemy::SetHealth(int NewHealth)
{
	int ClampedNewHealth = FMath::Clamp(NewHealth, 0, MaxHealth);
	if (ClampedNewHealth != Health)
	{
		Health = ClampedNewHealth;
		if(EnemyHealthBarComponent != nullptr)
		{
			EnemyHealthBarComponent->EnemyHealthBar->SetPercent(static_cast<float>(Health)/static_cast<float>(MaxHealth));
		}
	}
	if (Health == 0)
	{
		Destroy();
	}
}

int AUTAD_UI_FPS_Enemy::GetHealth()
{
	return Health;
}

void AUTAD_UI_FPS_Enemy::SetMaxHealth(int NewMaxHealth)
{
	MaxHealth = FMath::Clamp(NewMaxHealth, 0, NewMaxHealth);
}

int AUTAD_UI_FPS_Enemy::GetMaxHealth()
{
	return MaxHealth;
}