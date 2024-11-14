// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Crosshair.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class UTAD_UI_FPS_API UCrosshair : public UUserWidget
{
	GENERATED_BODY()

	
public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* Crosshair;

	bool FireAnimation = false;
	float BlinkTimer = 0.f;
	
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Show();

	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Hide();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	virtual bool Initialize() override;
};
