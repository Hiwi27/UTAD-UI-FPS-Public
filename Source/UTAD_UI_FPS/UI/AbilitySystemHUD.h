// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemHUD.generated.h"

UENUM()
enum class EAbilityTree
{
	Movement,
	Health,
	Damage
};

class UButton;
/**
 * 
 */
UCLASS()
class UTAD_UI_FPS_API UAbilitySystemHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UButton* ActualButton;

	UPROPERTY(BlueprintReadWrite, Category="Ability Tree")
	EAbilityTree ActualTree;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* MovementBtn;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* MovementBtn_1;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* MovementBtn_2;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* healthBtn;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* healthBtn_1;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* healthBtn_2;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* DamageBtn;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* DamageBtn_1;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* DamageBtn_2;

	UPROPERTY()
	TArray<UButton*> movementColumn;
	
	UPROPERTY()
	TArray<bool> movementColumnUnlocked;

	UPROPERTY()
	TArray<UButton*> healthColumn;

	UPROPERTY()
	TArray<bool> healthColumnUnlocked;

	UPROPERTY()
	TArray<UButton*> damageColumn;

	UPROPERTY()
	TArray<bool> damageColumnUnlocked;
	
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Show();

	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Hide();

	UFUNCTION(BlueprintCallable)
	void Up();

	UFUNCTION(BlueprintCallable)
	void Right();

	UFUNCTION(BlueprintCallable)
	void Down();

	UFUNCTION(BlueprintCallable)
	void Left();
	
	UFUNCTION(BlueprintCallable)
	void Enter();

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool Initialize() override;
};
