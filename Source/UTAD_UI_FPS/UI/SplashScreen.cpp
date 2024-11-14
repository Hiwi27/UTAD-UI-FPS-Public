// Fill out your copyright notice in the Description page of Project Settings.


#include "SplashScreen.h"

bool USplashScreen::Initialize()
{
	Super::Initialize();

	SetVisibility(ESlateVisibility::Hidden);
	return true;

}

void USplashScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void USplashScreen::Show()
{
	SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USplashScreen::Hide, 5.f);
}

void USplashScreen::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}
