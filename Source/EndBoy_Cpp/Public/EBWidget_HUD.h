// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBWidget_WeaponInfo.h"
#include "EBWidget_PickupMassage.h"
#include "EBWidgeT_HPBar.h"
#include "EBWidget_Map.h"
#include "EBWidget_Players.h"
#include "EBWidget_WeaponSlot.h"
#include "EBWidget_Inventory.h"
#include "EBWidget_ResultWindow.h"
#include "Blueprint/UserWidget.h"
#include "EBWidget_HUD.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API UEBWidget_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UEBWidget_WeaponInfo* WeaponInfo;

	UPROPERTY()
	UEBWidget_PickupMassage* PickupMassamge;

	UPROPERTY()
	UEBWidgeT_HPBar* HPBar;

	UPROPERTY()
	UEBWidget_ResultWindow* resultWindow;

	UPROPERTY(BlueprintReadOnly)
	UEBWidget_Inventory* inventory;

	UPROPERTY()
	UEBWidget_Players* players;

	UPROPERTY()
	TArray<UEBWidget_WeaponSlot*> weaponSlots;

	UPROPERTY()
	UEBWidget_Map* map;

public:
	UFUNCTION()
	void ShowResultWindow(int rank);

	UFUNCTION()
	void ConstructHUD();
};
