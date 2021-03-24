// Fill out your copyright notice in the Description page of Project Settings.

#include "EBWidgeT_HPBar.h"

void UEBWidgeT_HPBar::NativeConstruct() {
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_HP")));
	if (!HPProgressBar) {
		EBLOG(Warning, TEXT("Weaponinfo not created"));
	}
	else if (HPProgressBar) {
		BindHPBar(100.0f);
	}
}

void UEBWidgeT_HPBar::BindHPBar(float hp) {
	if (hp>=0.1f) {
		HPProgressBar->SetPercent(FMath::Clamp(hp / 100.0f, 0.0f, 1.0f));
	}
	else {
		HPProgressBar->SetPercent(0.0f);
	}
}