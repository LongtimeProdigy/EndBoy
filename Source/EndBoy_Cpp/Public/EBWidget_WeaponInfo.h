// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBWeapon.h"
#include "Blueprint/UserWidget.h"
#include "Components//TextBlock.h"
#include "EBWidget_WeaponInfo.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API UEBWidget_WeaponInfo : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UTextBlock* CurrentAmmo;

	UPROPERTY()
	UTextBlock* MaxAmmo;

	UPROPERTY()
	UTextBlock* shootMode;

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void BindWeapon(AEBWeapon* Weapon);	
};
