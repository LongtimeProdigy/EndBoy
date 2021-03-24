// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBExpandable.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "EBWidget_ClickSlot.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API UEBWidget_ClickSlot : public UUserWidget
{
	GENERATED_BODY()
		
public:
	UPROPERTY(BlueprintReadOnly)
	UButton* use_Button;

	UPROPERTY()
	UButton* drop_Button;

	UPROPERTY()
	UButton* quit_Button;

public:
	UPROPERTY(BlueprintReadWrite)
	AEBExpandable* targetItem;

private:
	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ClickUseButton();

	UFUNCTION()
	void ClickDropButton();

	UFUNCTION()
	void ClickQuitButton();
};
