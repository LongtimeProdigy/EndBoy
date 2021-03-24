// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBItem.h"
#include "EBWidget_HUD.h"
#include "GameFramework/PlayerController.h"
#include "EBPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API AEBPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	bool isinventoryopen;

	UPROPERTY()
	bool isMapOpen;

	UPROPERTY()
	FTimerHandle UsingTimerHandle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<class UEBWidget_HUD> Widget_HUD;

public:
	UPROPERTY(BlueprintReadOnly)
	UEBWidget_HUD* EB_HUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_SetInventory)
	TArray<AEBExpandable*> inventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = inventory)
	int countInventorySlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ping)
	FVector	targetPosition;

private:
	UFUNCTION()
	void PressInventory();

	UFUNCTION()
	void OnRep_SetInventory();

	UFUNCTION()
	void PressMap();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	AEBPlayerController(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	virtual void PostInitializeComponents() override;
	virtual void Possess(APawn* aPawn) override;

	UFUNCTION(Reliable, Client, WithValidation)
	void ClientShowResultWindow(int rank);
	void ClientShowResultWindow_Implementation(int rank);
	bool ClientShowResultWindow_Validate(int rank);

	UFUNCTION(Reliable, Server, WithValidation)
	void PickupItem(AEBExpandable* item);
	bool PickupItem_Validate(AEBExpandable* item);
	void PickupItem_Implementation(AEBExpandable* item);

	UFUNCTION(Reliable, Client, WithValidation)
	void ClientBindWeaponSlot(AEBWeapon* weapon, int slotnum);
	bool ClientBindWeaponSlot_Validate(AEBWeapon* weapon, int slotnum);
	void ClientBindWeaponSlot_Implementation(AEBWeapon* weapon, int slotnum);

	UFUNCTION(Reliable, Client, WithValidation)
	void ClientBindInventory();
	bool ClientBindInventory_Validate();
	void ClientBindInventory_Implementation();
};