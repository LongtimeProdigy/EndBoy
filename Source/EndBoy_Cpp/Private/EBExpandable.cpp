// Fill out your copyright notice in the Description page of Project Settings.

#include "EBExpandable.h"
#include "EBPlayerController.h"
#include "EBCharacter.h"

AEBExpandable::AEBExpandable() 
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicateMovement = true;

	ItemCategory = ItemCategory::EXPENDABLE;
}

//setup replication
void AEBExpandable::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEBExpandable, maxStack);
	DOREPLIFETIME(AEBExpandable, currentStack);
}

bool AEBExpandable::ServerPickupItem_Validate(ECollisionEnabled::Type newCollision, AActor* Character_pickup) {
	return Super::ServerPickupItem_Validate(newCollision, Character_pickup);
}
void AEBExpandable::ServerPickupItem_Implementation(ECollisionEnabled::Type newCollision, AActor* Character_pickup) {
	Super::ServerPickupItem_Implementation(newCollision, Character_pickup);

	MulticastPickupItem(newCollision, Character_pickup);
}

bool AEBExpandable::MulticastPickupItem_Validate(ECollisionEnabled::Type newCollision, AActor* Character_pickup) {
	return Super::MulticastPickupItem_Validate(newCollision, Character_pickup);
}
void AEBExpandable::MulticastPickupItem_Implementation(ECollisionEnabled::Type newCollision, AActor* Character_pickup) {
	Super::MulticastPickupItem_Implementation(newCollision, Character_pickup);

	//expandable item pickup function
	this->SetActorHiddenInGame(true);
}

bool AEBExpandable::ServerDropItem_Validate() {
	return Super::ServerDropItem_Validate();
}
void AEBExpandable::ServerDropItem_Implementation() {
	//expandable item pickup function
	this->SetActorHiddenInGame(false);

	if (Cast<AEBPlayerController>(GetOwner())->inventory.Contains(this)) {
		Cast<AEBPlayerController>(GetOwner())->inventory.Remove(this);
	}

	MulticastDropItem();

	Super::ServerDropItem_Implementation();
}
bool AEBExpandable::MulticastDropItem_Validate() {
	return Super::MulticastDropItem_Validate();
}
void AEBExpandable::MulticastDropItem_Implementation() {
	Super::MulticastDropItem_Implementation();
}

void AEBExpandable::Use() {
	
}

bool AEBExpandable::ServerUse_Validate() {
	return true;
}
void AEBExpandable::ServerUse_Implementation() {
	auto character = Cast<AEBCharacter>(Cast<AEBPlayerController>(GetOwner())->GetPawn());
	if (character->IsSprint) {
		character->ServerSprint();
	}
	if (character->IsAming) {
		character->ClientUnAming();
	}
	character->currentItem = this;
	character->MulticastPlayMontage(useMontage);
}
void AEBExpandable::ServerUsed() {
	auto controller = Cast<AEBPlayerController>(GetOwner());
	if (controller->inventory.Contains(this)) {
		AEBCharacter* character = Cast<AEBCharacter>(controller->GetPawn());
		character->currentItem = nullptr;
		if (character->currentMontage) {
			character->ServerStopMontage();
		}
		if (attachmentPart == EAttachmentPart::NOTATTACHMENT) {
			int index = controller->inventory.Find(this);
			if (currentStack != 0) {
				controller->ClientBindInventory();
			}
			else {
				controller->inventory.Remove(this);
			}
		}
		else {
			controller->inventory.Remove(this);
			this->SetActorHiddenInGame(false);
		}
	}
}

bool AEBExpandable::ServerUnequip_Validate() {
	return true;
}
void AEBExpandable::ServerUnequip_Implementation() {
	AEBPlayerController* controller = Cast<AEBPlayerController>(Owner_Controller);
	AEBCharacter* pawn = Cast<AEBCharacter>(controller->GetPawn());
	if (controller->inventory.Num() < 10) {
		controller->PickupItem(this);
	}
	else {
		ServerDropItem();
	}
	
	// uneuquip
	if (pawn->weapon1->parts.ReturnPartByInt((int)attachmentPart - 1)) {
		pawn->weapon1->parts.SetupPartByInt((int)attachmentPart - 1, nullptr);
	}
	else {
		if (pawn->weapon2->parts.ReturnPartByInt((int)attachmentPart - 1)) {
			pawn->weapon2->parts.SetupPartByInt((int)attachmentPart - 1, nullptr);
		}
		else {
			EBLOG(Warning, TEXT("Detached part is not exist in weaponslots from character"));
		}
	}
}