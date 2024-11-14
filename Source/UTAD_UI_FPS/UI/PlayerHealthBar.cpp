// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHealthBar.h"
#include "Components/ProgressBar.h"
#include "Math/Color.h"
#include "Styling/SlateColor.h"

#define BLINK_ANIMATION_TIME 1.f

void UPlayerHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(PlayerHealthBar->GetPercent() <= 0.25f)
	{
		BlinkTimer += InDeltaTime; // Avanza el temporizador.
		LowHealthBlink();
	}
}

void UPlayerHealthBar::Show()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPlayerHealthBar::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerHealthBar::UpdatePlayerHealthBar(int NewHealth, int MaxHealth)
{
	PlayerHealthBar->SetPercent(NewHealth/MaxHealth);
}

void UPlayerHealthBar::LowHealthBlink()
{
	// Tiempo de parpadeo en segundos.
	float BlinkTime = BLINK_ANIMATION_TIME / 2.0f;

	// Verifica si es tiempo de cambiar de color.
	if (BlinkTimer >= BlinkTime)
	{
		BlinkTimer = 0.f;
		bBlinkTurningRed = !bBlinkTurningRed;
	}

	// Define el color actual basado en bBlinkTurningRed.
	FSlateColor CurrentColor = bBlinkTurningRed ? FSlateColor(FLinearColor::Red) : OriginalColor;

	// Aplica el color a la barra de salud.
		
	PlayerHealthBar->WidgetStyle.BackgroundImage.TintColor = CurrentColor;
}

bool UPlayerHealthBar::Initialize()
{
	Super::Initialize();
	
	OriginalColor = PlayerHealthBar->GetWidgetStyle().BackgroundImage.TintColor;

	return true;
	
}
