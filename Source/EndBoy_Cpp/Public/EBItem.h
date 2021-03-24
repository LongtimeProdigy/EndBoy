// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "GameFramework/Actor.h"
#include "UnrealNetwork.h"
#include "EBItem.generated.h"

UENUM(BlueprintType)
enum class ItemCategory : uint8 {
	NOTHING=0 UMETA(DisplayName = "Nothing"),
	WEAPON=1 UMETA(DisplayName = "Weapon"),
	EXPENDABLE=2 UMETA(DisplayName = "Expendable"),
	INTERACTIVE=3 UMETA(DisplayName = "Interactive"),
};

UCLASS()
class ENDBOY_CPP_API AEBItem : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Body;

	UPROPERTY()
	bool Ispickuped;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Owner)
	ItemCategory ItemCategory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Owner)
	AController* Owner_Controller;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Setup2)
	UTexture2D* thumbnail;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Information)
	int index;

public:
	// Sets default values for this actor's properties
	AEBItem();

	UFUNCTION(Reliable, Server, BlueprintCallable, WithValidation)
	virtual void ServerPickupItem(ECollisionEnabled::Type newCollision, AActor* Character_pickup);
	virtual void ServerPickupItem_Implementation(ECollisionEnabled::Type newCollision, AActor* Character_pickup);
	virtual bool ServerPickupItem_Validate(ECollisionEnabled::Type newCollision, AActor* Character_pickup);

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	virtual void MulticastPickupItem(ECollisionEnabled::Type newCollision, AActor* Character_pickup);
	virtual void MulticastPickupItem_Implementation(ECollisionEnabled::Type newCollision, AActor* Character_pickup);
	virtual bool MulticastPickupItem_Validate(ECollisionEnabled::Type newCollision, AActor* Character_pickup);

	UFUNCTION(Reliable, Server, WithValidation)
	virtual void ServerDropItem();
	virtual void ServerDropItem_Implementation();
	virtual bool ServerDropItem_Validate();

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	virtual void MulticastDropItem();
	virtual void MulticastDropItem_Implementation();
	virtual bool MulticastDropItem_Validate();

};