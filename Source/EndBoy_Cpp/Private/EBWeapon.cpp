// Fill out your copyright notice in the Description page of Project Settings.

#include "EBWeapon.h"
#include "EBCharacter.h"

// Sets default values
AEBWeapon::AEBWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);
	SetReplicateMovement(true);

	slotNum = 0;

	ItemCategory = ItemCategory::WEAPON;
	canShoot = false;
	canReload = false;
	CameraBlendTime = 0.5f;

	WeaponCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	WeaponCamera->SetupAttachment(RootComponent);
	WeaponCamera->AddRelativeLocation(FVector(12.5f, 0.0f, 15.0f));

	currentShootMode = EShootMode::SINGLE;
	currentState = EState::IDLE;
}

void AEBWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEBWeapon, CurrentAmmo);
	DOREPLIFETIME(AEBWeapon, MaxAmmo);
	DOREPLIFETIME(AEBWeapon, currentShootMode);
	DOREPLIFETIME(AEBWeapon, currentState);
	DOREPLIFETIME(AEBWeapon, slotNum);
	DOREPLIFETIME(AEBWeapon, parts);
}

void AEBWeapon::BeginPlay() {
	Super::BeginPlay();
	if (Role == ROLE_Authority) {
		ServerSpawnBulletSetting();
	}
}

bool AEBWeapon::ServerSpawnBulletSetting_Validate() {
	return true;
}
void AEBWeapon::ServerSpawnBulletSetting_Implementation() {
	for (int32 i = 0; i < MaxAmmo; i++)
	{
		if (Bullet) {
			FActorSpawnParameters SpawnInfo;
			auto SpawnedBullet = GetWorld()->SpawnActor<AEBBullet>(Bullet, GetActorLocation() + FVector(i * 100, 0.0f, 0.0f), FRotator::ZeroRotator, SpawnInfo);
			SpawnedBullet->BulletDamage = WeaponDamage;
			Bullets.Add(SpawnedBullet);
			SpawnedBullet->SetActorHiddenInGame(true);
		}
		else {
			EBLOG(Warning, TEXT("bullet class cant found"));
		}
	}
}

bool AEBWeapon::ServerPickupItem_Validate(ECollisionEnabled::Type newCollision, AActor* Character_pickup) {
	return Super::ServerPickupItem_Validate(newCollision, Character_pickup);
}
void AEBWeapon::ServerPickupItem_Implementation(ECollisionEnabled::Type newCollision, AActor* Character_pickup) {
	Super::ServerPickupItem_Implementation(newCollision, Character_pickup);

	auto Pawntopickup = Cast<AEBCharacter>(Character_pickup);
	if (Pawntopickup != nullptr) {
		AttachToComponent(Pawntopickup->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_r_weapon"));
		canShoot = true;
		canReload = true;

		for (int i = 0; i < 5; i++) {
			if (parts.ReturnPartByInt(i)) {
				parts.ReturnPartByInt(i)->Owner_Controller = Pawntopickup->GetController();
				parts.ReturnPartByInt(i)->SetOwner(Pawntopickup->GetController());
			}
		}

		MulticastPickupItem(newCollision, Character_pickup);
	}
}

bool AEBWeapon::MulticastPickupItem_Validate(ECollisionEnabled::Type newCollision, AActor* Character_pickup) {
	return Super::MulticastPickupItem_Validate(newCollision, Character_pickup);
}
void AEBWeapon::MulticastPickupItem_Implementation(ECollisionEnabled::Type newCollision, AActor* Character_pickup) {
	Super::MulticastPickupItem_Implementation(newCollision, Character_pickup);
}

bool AEBWeapon::ServerDropItem_Validate() {
	return Super::ServerDropItem_Validate();
}
void AEBWeapon::ServerDropItem_Implementation() {
	if (currentState == EState::IDLE) {
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		canShoot = false;
		canReload = false;
		MulticastDropItem();
	}
	else
	{
		EBLOG(Warning, TEXT("Weapon cant drop Cuz Reload or Shooting"));
	}
	Super::ServerDropItem_Implementation();
}
bool AEBWeapon::MulticastDropItem_Validate() {
	return Super::MulticastDropItem_Validate();
}
void AEBWeapon::MulticastDropItem_Implementation() {
	Super::MulticastDropItem_Implementation();
}

bool AEBWeapon::ServerUnequip_Validate(int rl) {
	return true;
}
void AEBWeapon::ServerUnequip_Implementation(int rl) {
	auto targetPawn = Cast<AEBCharacter>(Owner_Controller->GetPawn());

	if (rl == 0) {
		AttachToComponent(targetPawn->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("back_r_weapon"));
	}
	else if(rl == 1) {
		AttachToComponent(targetPawn->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("back_l_weapon"));
	}

	targetPawn->CurrentWeapon = nullptr;
}

void AEBWeapon::DoFireStart() {
	
	switch (currentState)
	{
	case EState::IDLE:
		EBLOG(Warning, TEXT("IDLE"));
		break;
	case EState::RELOADING:
		EBLOG(Warning, TEXT("RELOADING"));
		break;
	case EState::SHOOTING:
		EBLOG(Warning, TEXT("SHOOTING"));
		break;
	default:
		break;
	}

	if (canShoot && currentState == EState::IDLE) {
		switch (currentShootMode)
		{
		case EShootMode::SINGLE:
			FireBullet();
			break;
		case EShootMode::BURST:
			FireBullet();
			GetWorldTimerManager().SetTimer(shootBurstTimerHandle1, this, &AEBWeapon::FireBullet, delayBurst * 1, false);
			GetWorldTimerManager().SetTimer(shootBurstTimerHandle2, this, &AEBWeapon::FireLastBurst, delayBurst * 2, false);

			break;
		case EShootMode::AUTO:
			FireBullet();
			GetWorldTimerManager().SetTimer(shootTimerHandle, this, &AEBWeapon::FireBullet, delayAuto, true, delayAuto);
			break;
		default:
			break;
		}
	}
	else {
		EBLOG(Warning, TEXT("shoot is failed"));
	}
}
void AEBWeapon::DoFireStop() {
	GetWorldTimerManager().ClearTimer(shootTimerHandle);
}
void AEBWeapon::TimerDoFireStop() {
	if (currentShootMode != EShootMode::BURST) {
		currentState = EState::IDLE;
		//canShoot = true;
	}
	else {
		EBLOG(Warning, TEXT("Mode : Burst"));
	}
}
void AEBWeapon::FireBullet() {
	if (CurrentAmmo != MaxAmmo) {
		if (Bullets[CurrentAmmo]) {
			Bullets[CurrentAmmo]->SetActorHiddenInGame(false);
			Bullets[CurrentAmmo]->SetupBullet(Body->GetSocketLocation(TEXT("Muzzle")), Body->GetSocketRotation(TEXT("Muzzle")), BulletSpeed, FName("Bullet"), Owner_Controller);
			CurrentAmmo++;

			currentState = EState::SHOOTING;

			Cast<APlayerController>(GetOwner())->ClientPlayCameraShake(WeaponCameraShake, 1.0f);

			MulticastFire();
			EBLOG(Warning, TEXT("Bullet(%d) is fired"), CurrentAmmo);

			if (currentShootMode != EShootMode::BURST) {
				GetWorldTimerManager().SetTimer(stopFireTimer, this, &AEBWeapon::TimerDoFireStop, shootDelay, false, shootDelay);
			}
		}
		else {
			EBLOG(Warning, TEXT("Bullet(%d) is empty"), CurrentAmmo);
		}
	}
	else {
		//CurrentAmmo = MaxAmmo;
		//currentState = EState::IDLE;
		// ... Spawn empty magnize sound
		EBLOG(Warning, TEXT("Magnize is empty. plz do reload"));
		DoFireStop();
	}
}
void AEBWeapon::FireLastBurst() {
	FireBullet();
	GetWorldTimerManager().ClearTimer(shootBurstTimerHandle1);
	GetWorldTimerManager().ClearTimer(shootBurstTimerHandle2);
	currentState = EState::IDLE;
}

bool AEBWeapon::MulticastFire_Validate() {
	return true;
}
void AEBWeapon::MulticastFire_Implementation() {
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particleforshoot, FTransform(GetActorRotation(), Body->GetSocketLocation(TEXT("Muzzle")), GetActorScale()));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Soundforshoot, Body->GetSocketLocation(TEXT("Muzzle")));
}

void AEBWeapon::ReloadComplete() {
	canShoot = true;
	canReload = true;
	currentState = EState::IDLE;
	
	CurrentAmmo = 0;

	EBLOG(Warning, TEXT("Now Weapon Reloading is complete"));
}

void AEBWeapon::MakeUnUsableWeapon() {
	canShoot = false;
	canReload = false;
	currentState = EState::RELOADING;

	EBLOG(Warning, TEXT("Now Weapon is unusable"));
}

void AEBWeapon::Zooming(APlayerController* Controller) {
	if (WeaponCamera && Controller && Controller->IsLocalPlayerController()) {
		Controller->SetViewTargetWithBlend(this, CameraBlendTime);

		EBLOG(Warning, TEXT("Zooming is complete"));
	}
	else {
		EBLOG(Warning, TEXT("Weapon dont have camera thats error"));
	}
}

void AEBWeapon::ChangeShootMode() {
	switch (currentShootMode)
	{
	case EShootMode::SINGLE:
		if (canBurst) {
			currentShootMode = EShootMode::BURST;
		}
		else {
			if (canAuto) {
				currentShootMode = EShootMode::AUTO;
			}
			else {
				return;
			}
		}
		break;
	case EShootMode::BURST:
		if (canAuto) {
			currentShootMode = EShootMode::AUTO;
		}
		else {
			currentShootMode = EShootMode::SINGLE;
		}
		break;
	case EShootMode::AUTO:
		currentShootMode = EShootMode::SINGLE;
		break;
	default:
		break;
	}
}

void AEBWeapon::EquipAttachment(AEBExpandable* attachment) {
	int intAttachment = (int)attachment->attachmentPart - 1;
	if (canEquipParts.ReturnByInt(intAttachment)) {
		parts.SetupPartByInt(intAttachment, attachment);
		AEBExpandable* returnattachment = parts.ReturnPartByInt(intAttachment);
		const UEnum* enumptr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAttachmentPart"), true);
		if (!enumptr) {
			EBLOG(Warning, TEXT("Enum is not exist"));
			return;
		}
#if WITH_EDITOR
		FString enumString = enumptr->GetDisplayNameTextByIndex(intAttachment + 1).ToString();
#else
		FString enumString = enumptr->GetEnumName(EnumValue);
#endif
		returnattachment->AttachToComponent(Body, FAttachmentTransformRules::SnapToTargetIncludingScale, *enumString);
		returnattachment->ServerUsed();
	}
}

void AEBWeapon::OnRep_SetParts() {
	if (GetOwner()) {
		Cast<AEBPlayerController>(GetOwner())->ClientBindWeaponSlot(this, slotNum);
	}
	else {
		EBLOG(Warning, TEXT("this is not exist"));
	}	
}

bool AEBWeapon::ServerLevelUp_Validate() {
	return true;
}
void AEBWeapon::ServerLevelUp_Implementation() {
	switch (currentLevel) {
		case 0:
			if (canLevelUp1) {
				if (level1Mesh.IsPending()) {
					UStaticMesh* mesh = level1Mesh.LoadSynchronous();
					MulticastLevelUp(mesh);
					currentLevel++;
				}
				else {
					MulticastLevelUp(level1Mesh.Get());
					currentLevel++;
					EBLOG(Warning, TEXT("Level1 Mesh already loaded"));
				}
				
			}
			break;
		case 1:
			if (canLevelUp2) {
				if (level2Mesh.IsPending()) {
					UStaticMesh* mesh = level2Mesh.LoadSynchronous();
					MulticastLevelUp(mesh);
					currentLevel++;
				}
				else {
					MulticastLevelUp(level2Mesh.Get());
					currentLevel++;
					EBLOG(Warning, TEXT("Level2 Mesh already loaded"));
				}
			}
			break;
		case 2:
			EBLOG(Warning, TEXT("Already weapon is Max Level"));
			break;
		default:
			break;
	}
}
bool AEBWeapon::MulticastLevelUp_Validate(UStaticMesh* mesh) {
	return true;
}
void AEBWeapon::MulticastLevelUp_Implementation(UStaticMesh* mesh) {
	Body->SetStaticMesh(mesh);
}

void AEBWeapon::OnRep_SetCanShoot() {
	EBLOG(Warning, TEXT("(%s)canshoot is changed"), Role == ROLE_Authority ? TEXT("Server") : TEXT("Client"));
}