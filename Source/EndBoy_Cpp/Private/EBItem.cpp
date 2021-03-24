// Fill out your copyright notice in the Description page of Project Settings.

#include "EBItem.h"

// Sets default values
AEBItem::AEBItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicateMovement = true;

	Ispickuped = false;
	ItemCategory = ItemCategory::NOTHING;

	Owner_Controller = nullptr;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;
	BoxCollision->SetCollisionProfileName(TEXT("Item"));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(BoxCollision);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool AEBItem::ServerPickupItem_Validate(ECollisionEnabled::Type newCollision, AActor* Character_pickup) {
	return true;
}
void AEBItem::ServerPickupItem_Implementation(ECollisionEnabled::Type newCollision, AActor* Character_pickup) {
	MulticastPickupItem(newCollision, Character_pickup);
}

bool AEBItem::MulticastPickupItem_Validate(ECollisionEnabled::Type newCollision, AActor* Character_pickup) {
	return true;
}
void AEBItem::MulticastPickupItem_Implementation(ECollisionEnabled::Type newCollision, AActor* Character_pickup) {
	BoxCollision->SetSimulatePhysics(false);
	BoxCollision->SetCollisionEnabled(newCollision);
}

bool AEBItem::ServerDropItem_Validate() {
	return true;
}
void AEBItem::ServerDropItem_Implementation() {
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	SetActorLocation(Cast<APlayerController>(GetOwner())->GetPawn()->GetActorLocation());
	BoxCollision->AddForce(Cast<APlayerController>(GetOwner())->GetPawn()->GetActorForwardVector() * 2000000.0f);	
}
bool AEBItem::MulticastDropItem_Validate() {
	return true;
}
void AEBItem::MulticastDropItem_Implementation() {
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetSimulatePhysics(true);
}