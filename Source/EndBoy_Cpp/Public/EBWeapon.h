// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBItem.h"
#include "EBExpandable.h"
#include "EBBullet.h"
#include "EBWeapon.generated.h"

class AEBCharacter;

UENUM(BlueprintType)
enum class EShootMode : uint8 {
	SINGLE UMETA(DisplayName = "Single"),
	BURST UMETA(DisplayName = "Boost"),
	AUTO UMETA(DisplayName = "Auto"),
};

UENUM(BlueprintType)
enum class EState :uint8 {
	IDLE UMETA(DisplayName = "Idle"),
	RELOADING UMETA(DisplayName = "Reloading"),
	SHOOTING UMETA(DisplayName = "Shooting"),
};

UENUM(BlueprintType)
enum class EPartsName :uint8 {
	NOTHING UMETA(DisplayName = "Idle"),
	MUZZLE UMETA(DisplayName = "Reloading"),
	GRIP UMETA(DisplayName = "Shooting"),
	MAGAZINE UMETA(DisplayName = "Shooting"),
	BUTT UMETA(DisplayName = "Shooting"),
	SIGHT UMETA(DisplayName = "Shooting"),
};

USTRUCT(Atomic, BlueprintType)
struct FCanEquipParts
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool canEquipMuzzle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool canEquipGrip;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool canEquipMagazine;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool canEquipButt;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool canEquipSight;

public:
	void Initialize(bool muzzle, bool grip, bool magazine, bool butt, bool sight) {
		canEquipMuzzle = muzzle;
		canEquipGrip = grip;
		canEquipMagazine = magazine;
		canEquipButt = butt;
		canEquipSight = sight;
		return;
	}

	bool ReturnByInt(int n) {
		switch (n)
		{
		case 0:
			return canEquipMuzzle;
			break;
		case 1:
			return canEquipGrip;
			break;
		case 2:
			return canEquipMagazine;
			break;
		case 3:
			return canEquipButt;
			break;
		case 4:
			return canEquipSight;
			break;
		default:
			return nullptr;
			break;
		}
	}
};

USTRUCT(Atomic, BlueprintType)
struct FParts 
{
	GENERATED_BODY()
public:
	UPROPERTY()
	AEBExpandable* muzzle;

	UPROPERTY()
	AEBExpandable* grip;

	UPROPERTY()
	AEBExpandable* magazine;

	UPROPERTY()
	AEBExpandable* butt;

	UPROPERTY()
	AEBExpandable* sight;

public:
	void SetupPartByInt(int n, AEBExpandable* item) {
		switch (n) {
		case 0:
			muzzle = item;
			break;
		case 1:
			grip = item;
			break;
		case 2:
			magazine = item;
			break;
		case 3:
			butt = item;
			break;
		case 4:
			sight = item;
			break;
		default:
			break;
		}
	}

	AEBExpandable* ReturnPartByInt(int n) {
		switch (n) {
		case 0:
			return muzzle;
			break;
		case 1:
			return grip;
			break;
		case 2:
			return magazine;
			break;
		case 3:
			return butt;
			break;
		case 4:
			return sight;
			break;
		default:
			return nullptr;
			break;
		}
	}
};

UCLASS()
class ENDBOY_CPP_API AEBWeapon : public AEBItem
{
	GENERATED_BODY()
private:
	UPROPERTY()
	UCameraComponent* WeaponCamera;

	UPROPERTY()
	FTimerHandle shootTimerHandle;

	UPROPERTY()
	FTimerHandle shootBurstTimerHandle1;

	UPROPERTY()
	FTimerHandle shootBurstTimerHandle2;

	UPROPERTY()
	FTimerHandle stopFireTimer;

protected:
	UPROPERTY(EditAnywhere, Category = Shoot)
	float shootDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shoot)
	bool canBurst;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shoot)
	bool canAuto;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = Shoot)
	float delayBurst;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = Shoot)
	float delayAuto;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shoot)
	TSubclassOf<UCameraShake> WeaponCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Level)
	FSoftObjectPath level1MeshPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Level)
	FSoftObjectPath level2MeshPath;

	UPROPERTY()
	TSoftObjectPtr<UStaticMesh> level1Mesh;

	UPROPERTY()
	TSoftObjectPtr<UStaticMesh> level2Mesh;

public:
	UPROPERTY(VisibleAnywhere, Replicated, Category = Slot)
	int slotNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bullet)
	TSubclassOf<AEBBullet> Bullet;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bullet)
	TArray<AEBBullet*> Bullets;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = Bullet)
	int32 MaxAmmo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Bullet)
	int32 CurrentAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bullet)
	int32 BulletSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bullet)
	UParticleSystem* Particleforshoot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bullet)
	USoundBase* Soundforshoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bullet, ReplicatedUsing = OnRep_SetCanShoot)
	bool canShoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bullet)
	bool canReload;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damage)
	float WeaponDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
	UAnimMontage* ReloadAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float CameraBlendTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	EShootMode currentShootMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	EState currentState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	FCanEquipParts canEquipParts;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_SetParts, Category = Parts)
	FParts parts;

	UPROPERTY(VisibleAnywhere, Category = Level)
	int currentLevel;

	UPROPERTY(EditAnywhere, Category = Level)
	bool canLevelUp1;

	UPROPERTY(EditAnywhere, Category = Level)
	bool canLevelUp2;

public:	
	// Sets default values for this actor's properties
	AEBWeapon();

	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
	virtual void ServerPickupItem_Implementation(ECollisionEnabled::Type newCollision, AActor* Character_pickup) override;
	virtual bool ServerPickupItem_Validate(ECollisionEnabled::Type newCollision, AActor* Character_pickup) override;
	
	virtual void MulticastPickupItem_Implementation(ECollisionEnabled::Type newCollision, AActor* Character_pickup) override;
	virtual bool MulticastPickupItem_Validate(ECollisionEnabled::Type newCollision, AActor* Character_pickup) override;

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerUnequip(int rl);
	void ServerUnequip_Implementation(int rl);
	bool ServerUnequip_Validate(int rl);
	
	virtual bool ServerDropItem_Validate() override;
	virtual void ServerDropItem_Implementation() override;

	virtual void MulticastDropItem_Implementation() override;
	virtual bool MulticastDropItem_Validate() override;

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSpawnBulletSetting();
	void ServerSpawnBulletSetting_Implementation();
	bool ServerSpawnBulletSetting_Validate();

	UFUNCTION()
	void DoFireStart();

	UFUNCTION()
	void DoFireStop();

	UFUNCTION()
	void TimerDoFireStop();

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void MulticastFire();
	void MulticastFire_Implementation();
	bool MulticastFire_Validate();

	UFUNCTION()
	void ReloadComplete();

	UFUNCTION()
	void MakeUnUsableWeapon();

	UFUNCTION()
	void Zooming(APlayerController* Controller);

	UFUNCTION()
	void ChangeShootMode();

	UFUNCTION()
	void FireBullet();

	UFUNCTION()
	void FireLastBurst();

	UFUNCTION()
	void EquipAttachment(AEBExpandable* attachment);

	UFUNCTION()
	void OnRep_SetParts();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerLevelUp();
	void ServerLevelUp_Implementation();
	bool ServerLevelUp_Validate();

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void MulticastLevelUp(UStaticMesh* mesh);
	void MulticastLevelUp_Implementation(UStaticMesh* mesh);
	bool MulticastLevelUp_Validate(UStaticMesh* mesh);

	UFUNCTION()
	void OnRep_SetCanShoot();
};