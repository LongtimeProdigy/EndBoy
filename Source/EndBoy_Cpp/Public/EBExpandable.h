// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBItem.h"
#include "EBExpandable.generated.h"

class AEBPlayerController;
class AEBCharacter;

UENUM(BlueprintType)
enum class EAttachmentPart : uint8 {
	NOTATTACHMENT UMETA(DisplayName = "NotAttachment"),
	MUZZLE UMETA(DisplayName = "Muzzle"),
	GRIP UMETA(DisplayName = "Grip"),
	MAGAZINE UMETA(DisplayName = "Magazine"),
	BUTT UMETA(DisplayName = "Butt"),
	SIGHT UMETA(DisplayName = "Sight"),
};

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API AEBExpandable : public AEBItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup1)
	EAttachmentPart attachmentPart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup1)
	bool isStackable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = Setup1)
	int maxStack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = Setup1)
	int currentStack;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle useTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* useMontage;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup1)
	float useDelay;

public:
	// Sets default values for this actor's properties
	AEBExpandable();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	virtual void ServerPickupItem_Implementation(ECollisionEnabled::Type newCollision, AActor* Character_pickup) override;
	virtual bool ServerPickupItem_Validate(ECollisionEnabled::Type newCollision, AActor* Character_pickup) override;

	virtual void MulticastPickupItem_Implementation(ECollisionEnabled::Type newCollision, AActor* Character_pickup) override;
	virtual bool MulticastPickupItem_Validate(ECollisionEnabled::Type newCollision, AActor* Character_pickup) override;

	virtual bool ServerDropItem_Validate() override;
	virtual void ServerDropItem_Implementation() override;

	virtual void MulticastDropItem_Implementation() override;
	virtual bool MulticastDropItem_Validate() override;

	UFUNCTION()
	virtual void Use();

	UFUNCTION(Reliable, Server, WithValidation)
	virtual void ServerUse();
	virtual bool ServerUse_Validate();
	virtual void ServerUse_Implementation();

	UFUNCTION()
	virtual void ServerUsed();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerUnequip();
	bool ServerUnequip_Validate();
	void ServerUnequip_Implementation();
};
