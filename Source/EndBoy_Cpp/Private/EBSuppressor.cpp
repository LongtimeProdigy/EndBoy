// Fill out your copyright notice in the Description page of Project Settings.

#include "EBSuppressor.h"
#include "EBCharacter.h"

AEBSuppressor::AEBSuppressor() {
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BODY(TEXT("StaticMesh'/Game/Objects/FPS_Weapon_Bundle/Weapons/Meshes/Accessories/SM_Vertgrip.SM_Vertgrip'"));
	if (BODY.Succeeded()) {
		Body->SetStaticMesh(BODY.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D>THUMBNAIL(TEXT("Texture2D'/Game/Objects/FPS_Weapon_Bundle/Weapons/Thumbnail/2026753_1.2026753_1'"));
	if (THUMBNAIL.Succeeded()) {
		thumbnail = THUMBNAIL.Object;
	}

	attachmentPart = EAttachmentPart::MUZZLE;
	useDelay = 0;
	isStackable = false;
	maxStack = 1;
	currentStack = 1;
	index = 1;
}

void AEBSuppressor::Use() {
	// ... 캐스팅 필요
	ServerUse();
}
bool AEBSuppressor::ServerUse_Validate() {
	return Super::ServerUse_Validate();
}
void AEBSuppressor::ServerUse_Implementation() {
	Super::ServerUse_Implementation();

	auto targetcharacter = Cast<AEBCharacter>(Owner_Controller->GetPawn());
	if (IsValid(targetcharacter->CurrentWeapon)) {
		if (targetcharacter->CurrentWeapon->parts.ReturnPartByInt((int)attachmentPart - 1)) {
			targetcharacter->CurrentWeapon->parts.ReturnPartByInt((int)attachmentPart - 1)->ServerUnequip(); // <- return serverused;
		}
		targetcharacter->CurrentWeapon->EquipAttachment(this);
	}
	else {
		EBLOG(Warning, TEXT("There are not weapon to attach attachment"))
	}
}