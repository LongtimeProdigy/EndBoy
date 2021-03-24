// Fill out your copyright notice in the Description page of Project Settings.

#include "EBWidget_ClickSlot.h"

void UEBWidget_ClickSlot::NativeConstruct() {
	Super::NativeConstruct();

	use_Button = Cast<UButton>(GetWidgetFromName("UseButton"));
	if (!use_Button) {
		EBLOG(Warning, TEXT("UseButton is not created"));
	}
	else {
		use_Button->OnClicked.AddDynamic(this, &UEBWidget_ClickSlot::ClickUseButton);
	}

	drop_Button = Cast<UButton>(GetWidgetFromName("DropButton"));
	if (!drop_Button) {
		EBLOG(Warning, TEXT("DropButton is not created"));
	}
	else {
		drop_Button->OnClicked.AddDynamic(this, &UEBWidget_ClickSlot::ClickDropButton);
	}

	quit_Button = Cast<UButton>(GetWidgetFromName("QuitButton"));
	if (!quit_Button) {
		EBLOG(Warning, TEXT("QuitButton is not created"));
	}
	else {
		quit_Button->OnClicked.AddDynamic(this, &UEBWidget_ClickSlot::ClickQuitButton);
	}
}

void UEBWidget_ClickSlot::ClickUseButton() {
	if (targetItem) {
		auto item = Cast<AEBExpandable>(targetItem);
		item->Use();
		SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		EBLOG(Warning, TEXT("item is not setted. plz check"));
	}
}

void UEBWidget_ClickSlot::ClickDropButton() {
	if (targetItem) {
		//auto item = Cast<AEBExpandable>(targetItem);
		targetItem->ServerDropItem();
		SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		EBLOG(Warning, TEXT("item is not setted. plz check"));
	}
}

void UEBWidget_ClickSlot::ClickQuitButton() {
	SetVisibility(ESlateVisibility::Hidden);
}