// Fill out your copyright notice in the Description page of Project Settings.

#include "EBWidget_WeaponInfo.h"

void UEBWidget_WeaponInfo::NativeConstruct() {

	CurrentAmmo = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentAmmo")));
	MaxAmmo = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxAmmo")));
	shootMode = Cast<UTextBlock>(GetWidgetFromName(TEXT("Mode")));

	if (CurrentAmmo != nullptr && MaxAmmo != nullptr && shootMode != nullptr) {
		BindWeapon(nullptr);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Contructor Error, WidgetName: %s"), *GetName());
	}
}

void UEBWidget_WeaponInfo::BindWeapon(AEBWeapon* Weapon) {

	if (Weapon != nullptr) {
		SetVisibility(ESlateVisibility::Visible);
		CurrentAmmo->SetText(FText::FromString(FString::FromInt(Weapon->MaxAmmo - Weapon->CurrentAmmo)));
		MaxAmmo->SetText(FText::FromString(FString::FromInt(Weapon->MaxAmmo)));
		switch (Weapon->currentShootMode)
		{
		case EShootMode::SINGLE:
			shootMode->SetText(FText::FromString("Single"));
			break;
		case EShootMode::BURST:
			shootMode->SetText(FText::FromString("Boost"));
			break;
		case EShootMode::AUTO:
			shootMode->SetText(FText::FromString("Auto"));
			break;
		default:
			break;
		}
	}
	else {
		SetVisibility(ESlateVisibility::Hidden);
	}
}