// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "EBBullet.generated.h"

UCLASS()
class ENDBOY_CPP_API AEBBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Setup)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Setup)
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Setup)
	UParticleSystemComponent* ParticlaComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Setup)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Setup)
	float BulletDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Setup)
	AController* Instigator_Controller;

protected:
	// Sets default values for this actor's properties
	AEBBullet();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Reliable, Server, WithValidation)
	void OnShpereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnShpereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	bool OnShpereBeginOverlap_Validate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupBullet(FVector Loc, FRotator Rot, float BulletSpeed, FName ProfileName, AController* Instigator_Con);
};
