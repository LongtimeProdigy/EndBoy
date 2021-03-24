// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBItem.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "EBWidget_PickupMassage.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API UEBWidget_PickupMassage : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UTextBlock* ItemName;

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void BindItem(AEBItem* Item_to_pickup, FString string);
};