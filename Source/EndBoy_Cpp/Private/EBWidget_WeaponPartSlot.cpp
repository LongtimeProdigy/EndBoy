// Fill out your copyright notice in the Description page of Project Settings.

#include "EBWidget_WeaponPartSlot.h"

void UEBWidget_WeaponPartSlot::NativeConstruct() {
	Super::NativeConstruct();

	item = nullptr;

	btn = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Slot")));
	if (!btn) {
		EBLOG(Warning, TEXT("WeaponPartSlot Button is not created"));
	}
	else {
		btn->OnClicked.AddDynamic(this, &UEBWidget_WeaponPartSlot::ButtonClicked);
	}

	img = Cast<UImage>(GetWidgetFromName(TEXT("Img_Slot")));
	if (!img) {
		EBLOG(Warning, TEXT("WeaponPartSlot Image is not created"));
	}
}

void UEBWidget_WeaponPartSlot::ButtonClicked() {
	if (item) {
		item->ServerUnequip();
	}
}

void UEBWidget_WeaponPartSlot::BindItem(AEBExpandable* itemfromweapon) {
	if (itemfromweapon) {
		item = itemfromweapon;
		img->SetBrushFromTexture(itemfromweapon->thumbnail);
	}
	else {
		img->SetBrushFromTexture(nullptr);
	}
}