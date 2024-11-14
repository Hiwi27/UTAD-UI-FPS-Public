// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class USplashScreen;
class UButton;
class UPlayerHitMarker;
class UAmmoCounter;
class UCrosshair;
class UPlayerHealthBar;
class UReloadBar;
class UPlayerHealthBar;

/**
 * 
 */
UCLASS()
class UTAD_UI_FPS_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UPlayerHealthBar* PlayerHealthBarWidget;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAmmoCounter* AmmoCounterWidget;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UCrosshair* CrosshairWidget;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UPlayerHitMarker* PlayerHitMarker;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UReloadBar* ReloadBarWidget;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	USplashScreen* SplashScreenWidget;

	/** Show the Minimap and the PlayerHealthBar */
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void ShowNoWeapon();

	/** Show all the widgets */
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void ShowAll();

	/** Hide all the widgets */
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Hide();

	virtual bool Initialize() override;

};
