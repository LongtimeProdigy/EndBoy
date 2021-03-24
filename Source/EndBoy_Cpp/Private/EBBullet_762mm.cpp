// Fill out your copyright notice in the Description page of Project Settings.

#include "EBBullet_762mm.h"

AEBBullet_762mm::AEBBullet_762mm() {

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BULLETMESH(TEXT("StaticMesh'/Game/Objects/FPS_Weapon_Bundle/Weapons/Meshes/Accessories/SM_Scope_25x56_X.SM_Scope_25x56_X'"));
	if (BULLETMESH.Succeeded()) {
		Body->SetStaticMesh(BULLETMESH.Object);
	}
	else {
		EBLOG(Warning, TEXT("BulletMesh load failed"));
	}
}