// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReloadBar.generated.h"
class UProgressBar;
/**
 * 
 */
UCLASS()
class UTAD_UI_FPS_API UReloadBar : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* ReloadBar;
	
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Show();

	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Hide();

	void UpdateReloadBarValue(float NewValue);
};
