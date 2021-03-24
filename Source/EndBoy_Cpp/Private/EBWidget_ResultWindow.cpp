// Fill out your copyright notice in the Description page of Project Settings.

#include "EBWidget_ResultWindow.h"

void UEBWidget_ResultWindow::NativeConstruct() {
	txtMassage = Cast<UTextBlock>(GetWidgetFromName(TEXT("txt_Massage")));
	if (!txtMassage) {
		EBLOG(Warning, TEXT("txtMassage widget is not created"));
	}

	txtRank = Cast<UTextBlock>(GetWidgetFromName(TEXT("txt_Rank")));
	if (!txtRank) {
		EBLOG(Warning, TEXT("txtRank widget is not created"));
	}
}