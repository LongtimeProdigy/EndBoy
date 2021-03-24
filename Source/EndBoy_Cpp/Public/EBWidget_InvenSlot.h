// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBExpandable.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "EBWidget_InvenSlot.generated.h"

class AEBPlayerController;

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API UEBWidget_InvenSlot : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UTextBlock* itemdesc;
	UPROPERTY()
	UTextBlock* itemCount;
	UPROPERTY()
	UImage* itemthumbnail;
	UPROPERTY()
	UButton* itembutton;

public:
	UPROPERTY(BlueprintReadOnly)
	AEBExpandable* slotitem;
	
private:
	UFUNCTION()
	virtual void NativeConstruct() override;
	UFUNCTION()
	void ClickedSlot();

public:
	UFUNCTION()
	void BindSlot(AEBExpandable* item);
};
