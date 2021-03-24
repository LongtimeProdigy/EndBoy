// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBExpandable.h"
#include "EBSuppressor.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API AEBSuppressor : public AEBExpandable
{
	GENERATED_BODY()

private:
	AEBSuppressor();

public:
	virtual void Use() override;

	virtual bool ServerUse_Validate() override;
	virtual void ServerUse_Implementation() override;
};
