// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBCharacter.h"
#include "EBPlayerController.h"
#include "EBPlayerController2.h"
#include "EBGameState.h"
#include "EBElectricField.h"
#include "GameFramework/GameModeBase.h"
#include "EBGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8 {
	START UMETA(DisplayName = "Start"),
	BATTLE UMETA(DisplayName = "BATTLE"),
	FINISH UMETA(DisplayName = "Finish"),
};

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API AEBGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	AEBGameState* EBGameState;

	UPROPERTY()
	TArray<AActor*> LoginedActors;

	UPROPERTY()
	TArray<AActor*> ArrivedActors;

	UPROPERTY()
	TArray<AActor*> DeadedActors;

	UPROPERTY(EditAnywhere, Category = Setup1)
	int countPhase;

	UPROPERTY()
	int currentPhase;

	UPROPERTY()
	FVector2D currentCirclePosition;

	UPROPERTY()
	FVector2D targetCirclePosition;

	UPROPERTY()
	float currentCircleRadius;

	UPROPERTY(EditAnywhere, Category = Setup1)
	TArray<float> circleRadius;

	UPROPERTY(EditAnywhere, Category = Setup1)
	TArray<int> timePhase;

	UPROPERTY(EditAnywhere, Category = Setup1)
	TArray<int> shrinkTime;

	UPROPERTY()
	EGameState currentGameState;

	UPROPERTY()
	FTimerHandle timerCirclePhase1;

	UPROPERTY()
	FTimerHandle timerCirclePhase2;
	FTimerDelegate timerDelegateCirclePhase;

	UPROPERTY()
	FVector2D tempPositionforLerp;

	UPROPERTY()
	float tempRadiusforLerp;

	UPROPERTY()
	AEBElectricField* electricfield;

	UPROPERTY()
	FTimerHandle timerBattleStart;

	UPROPERTY(EditAnywhere, Category = Setup1)
	int battleStartDelayTime;

private:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(Reliable, Server, WithValidation)
	void SpawnCharacter(APlayerController* newController);
	void SpawnCharacter_Implementation(APlayerController* newController);
	bool SpawnCharacter_Validate(APlayerController* newController);

	UFUNCTION()
	void BattleStart();

	UFUNCTION()
	void SettingCircle();

	UFUNCTION()
	void DecreaseMagneticField();

	UFUNCTION()
	void BattleStartCountDown();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	AEBGameMode();

	UFUNCTION()
	void PlayerDead(APlayerController* actor);

	UFUNCTION()
	int GetArrivedPlayerCount();
};
