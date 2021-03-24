// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBWidget_ClickSlot.h"
#include "EBWidget_InvenSlot.h"
#include "Blueprint/UserWidget.h"
#include "EBWidget_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API UEBWidget_Inventory : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<UEBWidget_InvenSlot*> slots;

	UPROPERTY(BlueprintReadOnly)
	UEBWidget_ClickSlot* clickwidget;

private:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void BindInventory(TArray<AEBExpandable*> items, int inventoryMaxCount);
};
