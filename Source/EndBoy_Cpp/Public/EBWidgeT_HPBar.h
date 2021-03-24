// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "EBWidgeT_HPBar.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API UEBWidgeT_HPBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UProgressBar* HPProgressBar;

public:
	UFUNCTION()
	void BindHPBar(float hp);

private:
	virtual void NativeConstruct() override;	
};
