
// Fill out your copyright notice in the Description page of Project Settings.

#include "EBWidget_InvenSlot.h"
#include "EBPlayerController.h"

void UEBWidget_InvenSlot::NativeConstruct() {
	Super::NativeConstruct();

	itemdesc = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
	if (!itemdesc) {
		EBLOG(Warning, TEXT("itemslot is not created"));
	}

	itemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_Count")));
	if (!itemCount) {
		EBLOG(Warning, TEXT("txt_count is not created"));
	}

	itemthumbnail = Cast<UImage>(GetWidgetFromName(TEXT("Thumbnail")));
	if (!itemthumbnail) {
		EBLOG(Warning, TEXT("itemthumbnail is not created"));
	}

	itembutton = Cast<UButton>(GetWidgetFromName("Button_0"));
	if (!itembutton) {
		EBLOG(Warning, TEXT("ItemButton is not created"));
	}
	else {
		//itembutton->OnClicked.AddDynamic(this, &UEBWidget_InvenSlot::ClickedSlot);
	}
}

void UEBWidget_InvenSlot::BindSlot(AEBExpandable* item) {
	if (item) {
		//AEBExpandable* expandable = Cast<AEBExpandable>(item);
		if (itemdesc) {
			itemdesc->SetText(FText::FromString(item->GetName()));
			itemCount->SetText(FText::FromString(FString::FromInt(item->currentStack)));
		}
		else {
			EBLOG(Warning, TEXT("itemdesc is not exist"));
		}
		
		if (itemthumbnail) {
			itemthumbnail->SetBrushFromTexture(item->thumbnail);
		}
		else {
			EBLOG(Warning, TEXT("itemthumbnail is not exist"));
		}

		slotitem = item;
	}
	else {
		if (itemdesc) {
			itemdesc->SetText(FText::FromString(""));
			itemCount->SetText(FText::FromString(""));
		}
		else {
			EBLOG(Warning, TEXT("itemdesc is not exist"));
		}

		if (itemthumbnail) {
			itemthumbnail->SetBrushFromTexture(nullptr);
		}
		else {
			EBLOG(Warning, TEXT("itemthumbnail is not exist"));
		}

		slotitem = nullptr;
	}
}

void UEBWidget_InvenSlot::ClickedSlot() {
	if (slotitem) {
		float mouseX = 0;
		float mouseY = 0;
		auto tempcontroller = Cast<AEBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		
		tempcontroller->GetMousePosition(mouseX, mouseY);
		EBLOG(Warning, TEXT("mouseX : %f, mouseY : %f"), mouseX, mouseY);

		tempcontroller->EB_HUD->inventory->clickwidget->SetVisibility(ESlateVisibility::Visible);
		//tempcontroller->EB_HUD->inventory->clickwidget->SetPositionInViewport(FVector2D(mouseX, mouseY));
		UCanvasPanelSlot* temppanel = Cast<UCanvasPanelSlot>(tempcontroller->EB_HUD->inventory->clickwidget->Slot);
		//temppanel->SetDesiredPosition(FVector2D(mouseX, mouseY));
		temppanel->SetPosition(FVector2D(mouseX, mouseY) - FVector2D(350, 260));
		tempcontroller->EB_HUD->inventory->clickwidget->targetItem = slotitem;
	}
}