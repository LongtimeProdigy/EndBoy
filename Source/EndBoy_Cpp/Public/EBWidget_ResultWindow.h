// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "EBWidget_ResultWindow.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API UEBWidget_ResultWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UTextBlock* txtMassage;

	UPROPERTY()
	UTextBlock* txtRank;
	
private:
	virtual void NativeConstruct() override;
};
