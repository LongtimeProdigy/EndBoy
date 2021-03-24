// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBExpandable.h"
#include "EBWeapon.h"
//#include "EBCharacter.h"
#include "EBUpgradeKit.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API AEBUpgradeKit : public AEBExpandable
{
	GENERATED_BODY()

private:
	AEBUpgradeKit();

public:
	virtual void Use() override;

	virtual bool ServerUse_Validate() override;
	virtual void ServerUse_Implementation() override;

	virtual void ServerUsed() override;
};
