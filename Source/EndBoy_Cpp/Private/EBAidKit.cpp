// Fill out your copyright notice in the Description page of Project Settings.

#include "EBAidKit.h"

AEBAidKit::AEBAidKit() {
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

	attachmentPart = EAttachmentPart::NOTATTACHMENT;
	percentAmount = 20;
	useDelay = 3;
	isStackable = true;
	maxStack = 10;
	currentStack = 1;
	index = 0;
}

void AEBAidKit::Use() {
	ServerUse();
}
bool AEBAidKit::ServerUse_Validate() {
	return Super::ServerUse_Validate();
}
void AEBAidKit::ServerUse_Implementation() {
	auto targetcharacter = Cast<AEBCharacter>(Owner_Controller->GetPawn());
	if (targetcharacter->HP < 100) {
		// Ä³½ºÆÃ
		Super::ServerUse_Implementation();
		GetWorldTimerManager().SetTimer(useTimer, this, &AEBExpandable::ServerUsed, useDelay, false, useDelay);
	}
	else {
		EBLOG(Warning, TEXT("Already Hp is full"));
	}
}

void AEBAidKit::ServerUsed() {
	auto targetcharacter = Cast<AEBCharacter>(Owner_Controller->GetPawn());

	targetcharacter->HP += targetcharacter->maxHP * (percentAmount / 100);
	targetcharacter->HP = FMath::Clamp(targetcharacter->HP, 0.0f, 100.0f);
	
	currentStack--;

	Super::ServerUsed();
}