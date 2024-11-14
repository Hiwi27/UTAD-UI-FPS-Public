// Fill out your copyright notice in the Description page of Project Settings.


#include "Crosshair.h"

#include "VectorTypes.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

#define BLINK_ANIMATION_TIME 0.5f


void UCrosshair::Show()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UCrosshair::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UCrosshair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(FireAnimation == true)
	{
		BlinkTimer += InDeltaTime; // Avanza el temporizador.

		// Tiempo de parpadeo en segundos.
		float BlinkTime = BLINK_ANIMATION_TIME / 4.0f;
		
		UCanvasPanelSlot* imageSlot = Cast<UCanvasPanelSlot>(Crosshair->Slot);
		imageSlot->SetSize(FVector2d(400,400));
		// Verifica si es tiempo de cambiar de color.
		if (BlinkTimer >= BlinkTime)
		{
			BlinkTimer = 0.f;
			imageSlot->SetSize(FVector2d(200,200));
			FireAnimation = false;
		}
	}
}

bool UCrosshair::Initialize()
{
	Super::Initialize();

	return true;
}
