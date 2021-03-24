// Fill out your copyright notice in the Description page of Project Settings.

#include "EBWidget_WeaponSlot.h"

void UEBWidget_WeaponSlot::NativeConstruct() {
	Super::NativeConstruct();

	txtName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_Name")));
	if (!txtName) {
		EBLOG(Warning, TEXT("txt_name is not created on enwidget_weaponslot"));
	}

	btnBackground = Cast<UButton>(GetWidgetFromName(TEXT("Btn_BackGround")));
	if (!btnBackground) {
		EBLOG(Warning, TEXT("btn_background is not created on enwidget_weaponslot"));
	}
	else {
		btnBackground->OnClicked.AddDynamic(this, &UEBWidget_WeaponSlot::ClickedButton);
	}

	imgBackground = Cast<UImage>(GetWidgetFromName(TEXT("Img_Thumbnail")));
	if (!imgBackground) {
		EBLOG(Warning, TEXT("img_Thumbnail is not created on ebwidget_weaponslot"));
	}

	for (int i = 0; i < 5; i++)
	{
		FString base = "UI_WeaponPartSlot";
		partSlots.Add(Cast<UEBWidget_WeaponPartSlot>(GetWidgetFromName(*base.Append(FString::FromInt(i)))));
		//EBLOG(Warning, *asd.Append(FString::FromInt(i)));
		if (IsValid(partSlots[i])) {
			partSlots[i]->SetVisibility(ESlateVisibility::Hidden);
		}
		else {
			EBLOG(Warning, TEXT("UI_WeaponPartSlot is not created on UI_WeaponSlot"));
		}
	}

	BindWeapon(nullptr);
}

void UEBWidget_WeaponSlot::BindWeapon(AEBWeapon* weaponfromcharacter) {
	if (weaponfromcharacter) {
		txtName->SetText(FText::FromString(FString(weaponfromcharacter->GetName())));
		imgBackground->SetBrushFromTexture(weaponfromcharacter->thumbnail);
		imgBackground->SetVisibility(ESlateVisibility::Visible);

		for (int i = 0; i < 5; i++)
		{
			if (weaponfromcharacter->canEquipParts.ReturnByInt(i)) {
				partSlots[i]->SetVisibility(ESlateVisibility::Visible);
				partSlots[i]->BindItem(weaponfromcharacter->parts.ReturnPartByInt(i));
			}
			else {
				partSlots[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	else {
		txtName->SetText(FText::FromString(FString("")));
		imgBackground->SetVisibility(ESlateVisibility::Hidden);
		for (int i = 0; i < 5; i++)
		{
			partSlots[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UEBWidget_WeaponSlot::ClickedButton() {
	EBLOG(Warning, TEXT("weaponslot is clicked"));
}