// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHitMarker.generated.h"

/**
 * 
 */
UCLASS()
class UTAD_UI_FPS_API UPlayerHitMarker : public UUserWidget
{
	GENERATED_BODY()

	float BlinkTimer = 0.f;

public:
	bool playerIsHit;
	float HitMarkerTime = 0.1f;

private:
	FTimerHandle HitMarkerTimeHandle;

public:
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Show();

	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Hide();

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool Initialize() override;

	virtual void NativeConstruct() override;
};
