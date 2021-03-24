// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBExpandable.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "EBWidget_WeaponPartSlot.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API UEBWidget_WeaponPartSlot : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	AEBExpandable* item;

	UPROPERTY()
	UButton* btn;

	UPROPERTY()
	UImage* img;

private:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ButtonClicked();

public:
	UFUNCTION()
	void BindItem(AEBExpandable* itemfromweapon);
};
