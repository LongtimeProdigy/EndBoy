// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBExpandable.h"
#include "EBCharacter.h"
#include "EBAidKit.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API AEBAidKit : public AEBExpandable
{
	GENERATED_BODY()

private:
	UPROPERTY()
	float percentAmount;												// % not amound
	
private:
	AEBAidKit();

public:
	virtual void Use() override;

	virtual bool ServerUse_Validate() override;
	virtual void ServerUse_Implementation() override;

	virtual void ServerUsed() override;
};