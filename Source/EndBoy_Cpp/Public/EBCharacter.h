// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBPlayerController.h"
#include "EBWeapon.h"
#include "GameFramework/Character.h"
#include "EBCharacter.generated.h"

class AEBGameMode;

UENUM(BlueprintType)
enum class Character_Pose : uint8 { 
	STAND UMETA(DisplayName = "Stand"),
	CROUCH UMETA(DisplayName = "Crouch"),
	PRONE UMETA(DisplayName = "Prone"),
	DEAD UMETA(DisplayName = "Dead"),
};

UCLASS()
class ENDBOY_CPP_API AEBCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Replicated, Category = Setup2)
	UAnimMontage* montageWeaponChange;

	UPROPERTY()
	FTimerHandle timerCircleDamage;
	FTimerDelegate delegateCircleDamage;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_SetHP, Category = Setup3)
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup3)
	float maxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Setup3)
	float ControllerPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Setup3)
	bool IsAming;

	UPROPERTY(VisibleAnywhere, Category = Setup1)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Setup1)
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool IsProne;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool IsCrouch;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool IsSprint;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool IsEquiped;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool IsFreeviewed;

	UPROPERTY(BlueprintReadOnly, Replicated)
	Character_Pose C_Pose;

	UPROPERTY(BlueprintReadOnly)
	AEBItem* Checked_Item;

	UPROPERTY(BlueprintReadOnly, Replicated)
	AEBWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Controller)
	AEBPlayerController* PawnController;

	UPROPERTY()
	FTimerHandle reloadTimerHandle;

	//UPROPERTY()
	FTimerDelegate weaponchangeTimerDelegate;

	UPROPERTY(Replicated)
	AEBWeapon* weapon1;

	UPROPERTY(Replicated)
	AEBWeapon* weapon2;

	UPROPERTY(Replicated)
	AEBExpandable* currentItem;

	UPROPERTY()
	UAnimMontage* currentMontage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

public:
	// Sets default values for this character's properties
	AEBCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	UFUNCTION()
	void MoveForward(float NewAxisValue);
	UFUNCTION()
	void MoveRight(float NewAxisValue);
	UFUNCTION()
	void Turn(float NewAxisValue);
	UFUNCTION()
	void LookUp(float NewAxisValue);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCrouching();
	void ServerCrouching_Implementation();
	bool ServerCrouching_Validate();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerProning();
	void ServerProning_Implementation();
	bool ServerProning_Validate();

	UFUNCTION()
	void Interactive();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerPickupItem(AEBItem* Itemtopickup);
	void ServerPickupItem_Implementation(AEBItem* Itemtopickup);
	bool ServerPickupItem_Validate(AEBItem* Itemtopickup);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerDropItem();
	void ServerDropItem_Implementation();
	bool ServerDropItem_Validate();

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void MulticastChange_Pose(Character_Pose To_Pose);
	void MulticastChange_Pose_Implementation(Character_Pose To_Pose);
	bool MulticastChange_Pose_Validate(Character_Pose To_Pose);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerDoFireStart();
	void ServerDoFireStart_Implementation();
	bool ServerDoFireStart_Validate();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerDoFireStop();
	void ServerDoFireStop_Implementation();
	bool ServerDoFireStop_Validate();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerReload();
	void ServerReload_Implementation();
	bool ServerReload_Validate();

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void MulticastPlayMontage(UAnimMontage* Montage);
	void MulticastPlayMontage_Implementation(UAnimMontage* Montage);
	bool MulticastPlayMontage_Validate(UAnimMontage* Montage);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerStopMontage();
	void ServerStopMontage_Implementation();
	bool ServerStopMontage_Validate();

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void MulticastStopMontage();
	void MulticastStopMontage_Implementation();
	bool MulticastStopMontage_Validate();

	UFUNCTION()
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetPitch(float Pitch);
	void ServerSetPitch_Implementation(float Pitch);
	bool ServerSetPitch_Validate(float Pitch);

	UFUNCTION()
	void ClickRightMouseButton();

	UFUNCTION()
	void Freeview();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSprint();
	void ServerSprint_Implementation();
	bool ServerSprint_Validate();

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void MulticastDead(FVector directionvector);
	void MulticastDead_Implementation(FVector directionvector);
	bool MulticastDead_Validate(FVector directionvector);

	UFUNCTION(Reliable, Client, WithValidation)
	void ClientDead(int rank);
	void ClientDead_Implementation(int rank);
	bool ClientDead_Validate(int rank);

	//UFUNCTION(Reliable, Server, WithValidation)
	//void Server_Healing(float amount);
	//void Server_Healing_Implementation(float amount);
	//bool Server_Healing_Validate(float amout);

	UFUNCTION(Reliable, Client, WithValidation)
	void ClientUnAming();
	void ClientUnAming_Implementation();
	bool ClientUnAming_Validate();

private:
	UFUNCTION(Reliable, Client, WithValidation)
	void ClientAming();
	void ClientAming_Implementation();
	bool ClientAming_Validate();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetIsAming(bool _IsAming);
	void ServerSetIsAming_Implementation(bool _IsAming);
	bool ServerSetIsAming_Validate(bool _IsAming);

	UFUNCTION()
	void OnRep_SetHP();

	UFUNCTION()
	void ReloadComplete();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerChangeShootMode();
	void ServerChangeShootMode_Implementation();
	bool ServerChangeShootMode_Validate();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerWeaponChange(int slotnum);
	void ServerWeaponChange_Implementation(int slotnum);
	bool ServerWeaponChange_Validate(int slotnum);

	template<int slotnum>
	void ServerWeaponChange() { ServerWeaponChange(slotnum); }

	UFUNCTION()
	void WeaponChangeComplete(int slotnum);

	//UFUNCTION(Reliable, Server, WithValidation)
	//void ServerWeaponInteraction();
	//void ServerWeaponInteraction_Implementation();
	//bool ServerWeaponInteraction_Validate();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCancleItem();
	void ServerCancleItem_Implementation();
	bool ServerCancleItem_Validate();

	//UFUNCTION(Reliable, Server, WithValidation)
	//void ServerBeginOverlapCircle(float damage, AActor* causor);
	//void ServerBeginOverlapCircle_Implementation(float damage, AActor* causor);
	//bool ServerBeginOverlapCircle_Validate(float damage, AActor* causor);

	//UFUNCTION()
	//void BeginOverlapCircle(float damage, AActor* causor);

	//UFUNCTION(Reliable, Server, WithValidation)
	//void ServerEndOverlapCircle();
	//void ServerEndOverlapCircle_Implementation();
	//bool ServerEndOverlapCircle_Validate();

	//UFUNCTION()
	//void EndOverlapCircle();

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void MulticastSetSpeed(float speed);
	void MulticastSetSpeed_Implementation(float speed);
	bool MulticastSetSpeed_Validate(float speed);

};