// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SplashScreen.generated.h"

/**
 * 
 */
UCLASS()
class UTAD_UI_FPS_API USplashScreen : public UUserWidget
{
	GENERATED_BODY()

	FTimerHandle TimerHandle;
	
	virtual bool Initialize() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Show();

	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Hide();


};
