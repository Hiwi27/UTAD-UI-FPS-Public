// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystemHUD.h"

#include "Components/Button.h"

void UAbilitySystemHUD::Show()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UAbilitySystemHUD::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UAbilitySystemHUD::Up()
{
	int count = 0;
	switch (ActualTree) {
	case EAbilityTree::Movement:
		for (UButton* Element : movementColumn)
		{
			if(ActualButton == Element && count > 0)
			{
				ActualButton = movementColumn[count -1 ];
				break;
			}
			if(ActualButton == Element && count == 0)
			{
				ActualButton = movementColumn.Last();
				break;
			}
			count++;
		}
		break;
	case EAbilityTree::Health:
		for (UButton* Element : healthColumn)
		{
			if(ActualButton == Element && count > 0)
			{
				ActualButton = healthColumn[count -1 ];
				break;
			}
			if(ActualButton == Element && count == 0)
			{
				ActualButton = healthColumn.Last();
				break;
			}
			count++;
		}
		break;
	case EAbilityTree::Damage:
		for (UButton* Element : damageColumn)
		{
			if(ActualButton == Element && count > 0)
			{
				ActualButton = damageColumn[count -1 ];
				break;
			}
			if(ActualButton == Element && count == 0)
			{
				ActualButton = damageColumn.Last();
				break;
			}
			count++;
		}
		break;
	}
}

void UAbilitySystemHUD::Right()
{
	switch (ActualTree) {
	case EAbilityTree::Movement:
		ActualButton = healthBtn;
		ActualTree = EAbilityTree::Health;
		break;
	case EAbilityTree::Health:
		ActualButton = DamageBtn;
		ActualTree = EAbilityTree::Damage;
		break;
	case EAbilityTree::Damage:
		ActualButton = MovementBtn;
		ActualTree = EAbilityTree::Movement;
		break;
	}
}

void UAbilitySystemHUD::Down()
{
	int count = 0;
	switch (ActualTree) {
	case EAbilityTree::Movement:
		for (UButton* Element : movementColumn)
		{
			if(ActualButton == Element && Element == movementColumn.Last())
			{
				ActualButton = movementColumn[0];
				break;
			}
			if(ActualButton == Element&& Element != movementColumn.Last())
			{
				ActualButton = movementColumn[count +1];
				break;
			}
			count++;
		}
		break;
	case EAbilityTree::Health:
		for (UButton* Element : healthColumn)
		{
			if(ActualButton == Element && Element == healthColumn.Last())
			{
				ActualButton = healthColumn[0];
				break;
			}
			if(ActualButton == Element&& Element != healthColumn.Last())
			{
				ActualButton = healthColumn[count +1];
				break;
			}
			count++;
		}
		break;
	case EAbilityTree::Damage:
		for (UButton* Element : damageColumn)
		{
			if(ActualButton == Element && Element == damageColumn.Last())
			{
				ActualButton = damageColumn[0];
				break;
			}
			if(ActualButton == Element&& Element != damageColumn.Last())
			{
				ActualButton = damageColumn[count +1];
				break;
			}
			count++;
		}
		break;
	}
}

void UAbilitySystemHUD::Left()
{
	switch (ActualTree) {
	case EAbilityTree::Movement:
		{
			ActualButton = DamageBtn;
			ActualTree = EAbilityTree::Damage;
			break;
		}
	case EAbilityTree::Health:
		{
			ActualButton = MovementBtn;
			ActualTree = EAbilityTree::Movement;
			break;
		}
	case EAbilityTree::Damage:
		{
			ActualButton = healthBtn;
			ActualTree = EAbilityTree::Health;
			break;
		}
	}
}

void UAbilitySystemHUD::Enter()
{
	int count = 0;
	switch (ActualTree) {
	case EAbilityTree::Movement:
		for (UButton* Element : movementColumn)
		{
			if(ActualButton == Element)
			{
				if(count > 0)
				{
					if(movementColumnUnlocked[count-1] == true)
					{
						movementColumnUnlocked[count] = true;
					}
				}

				if(count ==0)
				{
					movementColumnUnlocked[count] = true;
				}
				break;
			}
			
			count++;
		}
		break;
	case EAbilityTree::Health:
		for (UButton* Element : healthColumn)
		{
			if(ActualButton == Element)
			{
				if(count > 0)
				{
					if(healthColumnUnlocked[count-1] == true)
					{
						healthColumnUnlocked[count] = true;
					}
				}

				if(count ==0)
				{
					healthColumnUnlocked[count] = true;
				}
				break;
			}
			count++;
		}
		break;
	case EAbilityTree::Damage:
		for (UButton* Element : damageColumn)
		{
			if(ActualButton == Element)
			{
				if(count > 0)
				{
					if(damageColumnUnlocked[count-1] == true)
					{
						damageColumnUnlocked[count] = true;
					}
				}

				if(count ==0)
				{
					damageColumnUnlocked[count] = true;
				}
				break;
			}
			count++;
		}
		break;
	}
}

void UAbilitySystemHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ActualButton->SetRenderOpacity(1.0f);
	int count = 0;
	for (UButton* Element : movementColumn)
	{
		if(Element != ActualButton)
		{
			Element->SetRenderOpacity(0.5f);
		}

		if(movementColumnUnlocked[count] == true)
		{
			Element->SetBackgroundColor(FLinearColor(FColor::Green));
		}
		count++;
	}
	count = 0;
	for (UButton* Element : healthColumn)
	{
		if(Element != ActualButton)
		{
			Element->SetRenderOpacity(0.5f);

		}
		if(healthColumnUnlocked[count] == true)
		{
			Element->SetBackgroundColor(FLinearColor(FColor::Green));
		}
		count++;
	}
	count = 0;
	for (UButton* Element : damageColumn)
	{
		if(Element != ActualButton)
		{
			Element->SetRenderOpacity(0.5f);

		}
		if(damageColumnUnlocked[count] == true)
		{
			Element->SetBackgroundColor(FLinearColor(FColor::Green));
		}
		count++;
	}
}

bool UAbilitySystemHUD::Initialize()
{
	Super::Initialize();



	movementColumn.Add(MovementBtn);
	movementColumn.Add(MovementBtn_1);
	movementColumn.Add(MovementBtn_2);
	healthColumn.Add(healthBtn);
	healthColumn.Add(healthBtn_1);
	healthColumn.Add(healthBtn_2);
	damageColumn.Add(DamageBtn);
	damageColumn.Add(DamageBtn_1);
	damageColumn.Add(DamageBtn_2);

	movementColumnUnlocked.Add(false);
	movementColumnUnlocked.Add(false);
	movementColumnUnlocked.Add(false);
	healthColumnUnlocked.Add(false);
	healthColumnUnlocked.Add(false);
	healthColumnUnlocked.Add(false);
	damageColumnUnlocked.Add(false);
	damageColumnUnlocked.Add(false);
	damageColumnUnlocked.Add(false);

	ActualButton = movementColumn[0];

	ActualTree = EAbilityTree::Movement;




	

	return true;
}
