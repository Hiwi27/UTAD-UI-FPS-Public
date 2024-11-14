// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHitMarker.h"


#define BLINK_ANIMATION_TIME 1.f


void UPlayerHitMarker::Show()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	GetWorld()->GetTimerManager().SetTimer(HitMarkerTimeHandle, this, &UPlayerHitMarker::Hide, HitMarkerTime);
}

void UPlayerHitMarker::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerHitMarker::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}


bool UPlayerHitMarker::Initialize()
{
	Super::Initialize();

	Hide();

	return true;
}

void UPlayerHitMarker::NativeConstruct()
{
	Super::NativeConstruct();

}
