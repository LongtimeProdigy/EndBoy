// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "EBWidget_Players.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API UEBWidget_Players : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UTextBlock* textCount;

private:
	virtual void NativeConstruct() override;
};
