// Fill out your copyright notice in the Description page of Project Settings.

#include "EBWidget_PickupMassage.h"

void UEBWidget_PickupMassage::NativeConstruct() {

	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txt_ItemName")));

	if (ItemName != nullptr) {
		BindItem(nullptr, "");
	}
	else {
		EBLOG(Warning, TEXT("%s error, WidgetName: %s"), *GetName());
	}
}

void UEBWidget_PickupMassage::BindItem(AEBItem* Item_to_pickup, FString string) {

	if (Item_to_pickup != nullptr) {
		SetVisibility(ESlateVisibility::Visible);
		ItemName->SetText(FText::FromString(string.Append(Item_to_pickup->GetName())));
	}
	else {
		SetVisibility(ESlateVisibility::Hidden);
	}
}