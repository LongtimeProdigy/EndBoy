// Fill out your copyright notice in the Description page of Project Settings.

#include "EBWeapon_AK47X.h"

AEBWeapon_AK47X::AEBWeapon_AK47X() : AEBWeapon() {

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	MaxAmmo = 30;
	CurrentAmmo = 0;
	BulletSpeed = 50.0f;
	WeaponDamage = 22.0f;
	CameraBlendTime = 0.25f;
	delayBurst = 0.075f;
	delayAuto = 0.085f;
	shootDelay = 0.1f;
	index = 0;

	canEquipParts.Initialize(true, true, true, true, true);

	canBurst = true;
	canAuto = true;

	canLevelUp1 = true;
	canLevelUp2 = true;
	currentLevel = 0;

	Bullet = AEBBullet_762mm::StaticClass();

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BODY(TEXT("StaticMesh'/Game/Objects/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SM_KA47_X.SM_KA47_X'"));
	if (BODY.Succeeded()) {
		Body->SetStaticMesh(BODY.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>SHOOTPARTICLE(TEXT("ParticleSystem'/Game/Objects/WeaponComponent/UnnecessaryContent/Effects/PSystems/P_Muzzle.P_Muzzle'"));
	if (SHOOTPARTICLE.Succeeded()) {
		Particleforshoot = SHOOTPARTICLE.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>SHOOTAUDIO(TEXT("SoundWave'/Game/Objects/WeaponComponent/UnnecessaryContent/Audio/FPTWeaponFire02.FPTWeaponFire02'"));
	if (SHOOTAUDIO.Succeeded()) {
		Soundforshoot = SHOOTAUDIO.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>RELOADMONTAGE(TEXT("AnimMontage'/Game/Mannequin/Character/Animation/Reload_Rifle_Hip_Montage.Reload_Rifle_Hip_Montage'"));
	if (RELOADMONTAGE.Succeeded()) {
		ReloadAnim = RELOADMONTAGE.Object;
	}

	static ConstructorHelpers::FClassFinder<class UCameraShake>CAMERASHAKE(TEXT("Class'/Script/EndBoy_Cpp.Weapon_CameraShake'"));
	if (CAMERASHAKE.Succeeded()) {
		WeaponCameraShake = CAMERASHAKE.Class;
	}
	
	level1MeshPath.SetPath("StaticMesh'/Game/Objects/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SM_AR4_X.SM_AR4_X'");
	level1Mesh = level1MeshPath;

	level2MeshPath.SetPath("StaticMesh'/Game/Objects/FPS_Weapon_Bundle/Weapons/Meshes/KA_Val/SM_KA_Val_X.SM_KA_Val_X'");
	level2Mesh = level2MeshPath;
}