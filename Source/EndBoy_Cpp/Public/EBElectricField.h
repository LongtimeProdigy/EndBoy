// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBGameState.h"
#include "EBCharacter.h"
#include "GameFramework/Actor.h"
#include "EBElectricField.generated.h"

UCLASS()
class ENDBOY_CPP_API AEBElectricField : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* body;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* collision;

	UPROPERTY()
	float circleRadius;

	UPROPERTY()
	FVector circlePosition;

	UPROPERTY()
	AEBGameState* gameState;

	UPROPERTY()
	TArray<AEBCharacter*> outsideActors;

	UPROPERTY(EditAnywhere, Category = damage)
	float damage;

	UPROPERTY(EditAnywhere, Category = damage)
	float damageCycleTime;

	UPROPERTY()
	FTimerHandle timerCircleDamage;

	UPROPERTY()
	FTimerHandle timerCircleTransform;

private:
	UFUNCTION()
	void SettingElectricField();

	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	// Sets default values for this actor's properties
	AEBElectricField();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerStartElectricField();
	void ServerStartElectricField_Implementation();
	bool ServerStartElectricField_Validate();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerEndElectricField();
	void ServerEndElectricField_Implementation();
	bool ServerEndElectricField_Validate();
};