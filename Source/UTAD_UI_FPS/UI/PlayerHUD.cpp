// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Crosshair.h"
#include "AmmoCounter.h"
#include "PlayerHealthBar.h"
#include "PlayerHitMarker.h"
#include "ReloadBar.h"
#include "Components/Button.h"

void UPlayerHUD::ShowNoWeapon()
{

}

void UPlayerHUD::ShowAll()
{
	PlayerHealthBarWidget->Show();
	AmmoCounterWidget->Show();
	CrosshairWidget->Show();
	// PlayerHitMarker->Show();
	ReloadBarWidget->Show();
}

void UPlayerHUD::Hide()
{
	PlayerHealthBarWidget->Hide();
	AmmoCounterWidget->Hide();
	CrosshairWidget->Hide();
	// PlayerHitMarker->Hide();
	ReloadBarWidget->Hide();
}

bool UPlayerHUD::Initialize()
{
	 Super::Initialize();


	return true;
}
