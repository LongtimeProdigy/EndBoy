// Fill out your copyright notice in the Description page of Project Settings.

#include "EBUpgradeKit.h"
#include "EBCharacter.h"

AEBUpgradeKit::AEBUpgradeKit() {
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BODY(TEXT("StaticMesh'/Game/Objects/FPS_Weapon_Bundle/Weapons/Meshes/Accessories/SM_Vertgrip.SM_Vertgrip'"));
	if (BODY.Succeeded()) {
		Body->SetStaticMesh(BODY.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D>THUMBNAIL(TEXT("Texture2D'/Game/Objects/FPS_Weapon_Bundle/Weapons/Thumbnail/2026753_1.2026753_1'"));
	if (THUMBNAIL.Succeeded()) {
		thumbnail = THUMBNAIL.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>ANIMMONTAGE(TEXT("AnimMontage'/Game/Mannequin/Character/Animation/Reload_Rifle_Hip_Montage.Reload_Rifle_Hip_Montage'"));
	if (ANIMMONTAGE.Succeeded()) {
		useMontage = ANIMMONTAGE.Object;
	}

	attachmentPart = EAttachmentPart::NOTATTACHMENT;
	useDelay = 8;
	isStackable = false;
	maxStack = 1;
	currentStack = 1;
	index = 1;
}

void AEBUpgradeKit::Use() {
	ServerUse();
}

bool AEBUpgradeKit::ServerUse_Validate() {
	return Super::ServerUse_Validate();
}
void AEBUpgradeKit::ServerUse_Implementation() {
	auto targetcharacter = Cast<AEBCharacter>(Owner_Controller->GetPawn());
	auto targetWeapon = Cast<AEBWeapon>(targetcharacter->CurrentWeapon);

	if (targetWeapon) {
		switch (targetWeapon->currentLevel)
		{
		case 0:
			if (targetWeapon->canLevelUp1) {
				Super::ServerUse_Implementation();
				GetWorldTimerManager().SetTimer(useTimer, this, &AEBExpandable::ServerUsed, useDelay, false, useDelay);
			}
			break;
		case 1:
			if (targetWeapon->canLevelUp2) {
				Super::ServerUse_Implementation();
				GetWorldTimerManager().SetTimer(useTimer, this, &AEBExpandable::ServerUsed, useDelay, false, useDelay);
			}
			break;
		case 2:
			EBLOG(Warning, TEXT("Weapon level is already max"));
			break;
		default:
			EBLOG(Warning, TEXT("some error"));
			break;
		}
	}
	else {
		EBLOG(Warning, TEXT("Plz equip the weapon"));
	}
}

void AEBUpgradeKit::ServerUsed() {
	auto targetcharacter = Cast<AEBCharacter>(Owner_Controller->GetPawn());
	auto targetWeapon = Cast<AEBWeapon>(targetcharacter->CurrentWeapon);

	targetWeapon->ServerLevelUp();
	currentStack--;
	Super::ServerUsed();

	EBLOG(Warning, TEXT("UpgradeKit used"));
}