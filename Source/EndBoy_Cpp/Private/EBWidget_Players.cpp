// Fill out your copyright notice in the Description page of Project Settings.

#include "EBWidget_Players.h"

void UEBWidget_Players::NativeConstruct() {
	Super::NativeConstruct();

	textCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("NumText")));
}