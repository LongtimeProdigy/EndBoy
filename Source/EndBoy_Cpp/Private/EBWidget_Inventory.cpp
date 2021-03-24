
// Fill out your copyright notice in the Description page of Project Settings.

#include "EBWidget_Inventory.h"
#include "EBWidget_InvenSlot.h"

void UEBWidget_Inventory::NativeConstruct() {

	slots.Empty();

	for (int32 i = 0; i < 10; i++) {
		slots.Add(Cast<UEBWidget_InvenSlot>(GetWidgetFromName(*FString::Printf(TEXT("UI_InvenSlot_%d"), i))));
	}

	clickwidget = Cast<UEBWidget_ClickSlot>(GetWidgetFromName(TEXT("UI_ClickWidget")));
	if (clickwidget) {
		clickwidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		EBLOG(Warning, TEXT("clickwidget is not created on inventory"));
	}
}

void UEBWidget_Inventory::BindInventory(TArray<AEBExpandable*> items, int inventoryMaxCount) {
	for (int i=0; i < inventoryMaxCount; i++) {
		if (slots[i]) {
			slots[i]->BindSlot(NULL);
		}
	}

	for (int32 i = 0; i < items.Num(); i++)
	{
		if (items[i] != nullptr) {
			slots[i]->BindSlot(items[i]);
		}
	}
}