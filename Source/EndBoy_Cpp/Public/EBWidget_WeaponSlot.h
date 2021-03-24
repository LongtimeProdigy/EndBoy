// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBWeapon.h"
#include "EBWidget_WeaponPartSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "EBWidget_WeaponSlot.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API UEBWidget_WeaponSlot : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	AEBWeapon* weapon;

	UPROPERTY()
	UTextBlock* txtName;

	UPROPERTY()
	UButton* btnBackground;

	UPROPERTY()
	UImage* imgBackground;

	UPROPERTY()
	TArray<UEBWidget_WeaponPartSlot*> partSlots;

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void BindWeapon(AEBWeapon* weaponformcharacter);

	UFUNCTION()
	void ClickedButton();
};
