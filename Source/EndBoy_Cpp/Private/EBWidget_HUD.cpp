// Fill out your copyright notice in the Description page of Project Settings.

#include "EBWidget_HUD.h"

void UEBWidget_HUD::ConstructHUD() {
	WeaponInfo = Cast<UEBWidget_WeaponInfo>(GetWidgetFromName(TEXT("UI_WeaponInfo")));
	if (!WeaponInfo) {
		EBLOG(Warning, TEXT("Weaponinfo not created"));
	}

	PickupMassamge = Cast<UEBWidget_PickupMassage>(GetWidgetFromName(TEXT("UI_PickUpMassage")));
	if (!PickupMassamge) {
		EBLOG(Warning, TEXT("PickupMassamge not created"));
	}

	HPBar = Cast<UEBWidgeT_HPBar>(GetWidgetFromName(TEXT("UI_HPBar")));
	if (!HPBar) {
		EBLOG(Warning, TEXT("HPBar not created"));
	}

	inventory = Cast<UEBWidget_Inventory>(GetWidgetFromName(TEXT("UI_Inventory")));
	if (!inventory) {
		EBLOG(Warning, TEXT("Inventory not created"));
	}
	else {
		inventory->SetVisibility(ESlateVisibility::Hidden);
	}

	resultWindow = Cast<UEBWidget_ResultWindow>(GetWidgetFromName(TEXT("UI_Death")));
	if (!resultWindow) {
		EBLOG(Warning, TEXT("Deathinfo not created"));
	}
	else {
		resultWindow->SetVisibility(ESlateVisibility::Hidden);
	}

	players = Cast<UEBWidget_Players>(GetWidgetFromName(TEXT("UI_Players")));
	if (!players) {
		EBLOG(Warning, TEXT("Players is not created"));
	}

	/*for (int i = 0; i < 2; i++) {
		weaponSlots.Add(Cast<UEBWidget_WeaponSlot>(GetWidgetFromName(FText::Format(TEXT("UI_WeaponSlot{0}"), NSLOCTEXT(i)))));
		if (!weaponSlots[i]) {
			EBLOG(Warning, TEXT("WeaponSlot%d is not created on HUD"), i);
			EBLOG(Warning, TEXT("UI_WeaponSlot" + i));
		}
		else {
			weaponSlots[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}*/

	weaponSlots.Add(Cast<UEBWidget_WeaponSlot>(GetWidgetFromName(TEXT("UI_WeaponSlot0"))));
	if (!weaponSlots[0]) {
		EBLOG(Warning, TEXT("WeaponSlot0 is not created on HUD"));
	}
	else {
		weaponSlots[0]->SetVisibility(ESlateVisibility::Hidden);
	}

	weaponSlots.Add(Cast<UEBWidget_WeaponSlot>(GetWidgetFromName(TEXT("UI_WeaponSlot1"))));
	if (!weaponSlots[1]) {
		EBLOG(Warning, TEXT("WeaponSlot1 is not created on HUD"));
	}
	else {
		weaponSlots[1]->SetVisibility(ESlateVisibility::Hidden);
	}

	map = Cast<UEBWidget_Map>(GetWidgetFromName(TEXT("UI_Minimap")));
	if (!map) {
		EBLOG(Warning, TEXT("Map is not created"));
	}
	else {
		map->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UEBWidget_HUD::ShowResultWindow(int rank) {
	WeaponInfo->SetVisibility(ESlateVisibility::Hidden);
	PickupMassamge->SetVisibility(ESlateVisibility::Hidden);
	HPBar->SetVisibility(ESlateVisibility::Hidden);
	inventory->SetVisibility(ESlateVisibility::Hidden);
	players->SetVisibility(ESlateVisibility::Hidden);
	map->SetVisibility(ESlateVisibility::Hidden);
	for (int i = 0; i < weaponSlots.Num(); i++) {
		weaponSlots[i]->SetVisibility(ESlateVisibility::Hidden);
	}

	if (rank == 1) {
		resultWindow->txtMassage->SetText(FText::FromString("DINER DINER CHICKEN DINER"));
	}
	else {
		resultWindow->txtMassage->SetText(FText::FromString("THAT'S OKAY. NEXT AGAIN."));
	}
	FString rankmassage = TEXT("Rank: ");
	rankmassage.AppendInt(rank);
	resultWindow->txtRank->SetText(FText::FromString(rankmassage));
	resultWindow->SetVisibility(ESlateVisibility::Visible);

	//EBLOG(Warning, TEXT("resultWindow massage: %s"), resultWindow->txtMassage->GetText());
	//EBLOG(Warning, TEXT("resultWindow rank: %s"));
}