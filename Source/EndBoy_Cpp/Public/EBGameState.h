// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "EBPlayerController.h"
#include "GameFramework/GameState.h"
#include "EBGameState.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API AEBGameState : public AGameStateBase
{
	GENERATED_BODY()

private:

	UPROPERTY()
	FTimerHandle timerCirclePhase;

	//UPROPERTY()
	FTimerDelegate timerDelegateCirclePhase;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_SetArrivedPlayerCount)
	int arrivedPlayerCount;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SetBattleStartDelayTime)
	int battleStartCountDown;

	UPROPERTY(BlueprintReadOnly, Replicated)
	FVector2D currentCirclePosition;

	UPROPERTY(BlueprintReadOnly, Replicated)
	FVector2D targetCirclePosition;

	UPROPERTY(BlueprintReadOnly, Replicated)
	float currentCircleRadius;

	UPROPERTY(BlueprintReadOnly, Replicated)
	float targetCircleRadius;

	UPROPERTY(BlueprintReadOnly, Replicated)
	int phaseTime;

	UPROPERTY(BlueprintReadOnly, Replicated)
	int shrinkTime;

private:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_SetArrivedPlayerCount();

	UFUNCTION()
	void OnRep_SetBattleStartDelayTime();

public:
	UFUNCTION(Reliable, Server, WithValidation)
	void SettingCircle(FVector2D circlePosition, FVector2D nextCirclePosition, float circleRadius, float nextCircleRadius, int timephase, int timeshrink);
	void SettingCircle_Implementation(FVector2D circlePosition, FVector2D nextCirclePosition, float circleRadius, float nextCircleRadius, int timephase, int timeshrink);
	bool SettingCircle_Validate(FVector2D circlePosition, FVector2D nextCirclePosition, float circleRadius, float nextCircleRadius, int timephase, int timeshrink);

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void MulticastDecreaseCircle(FVector2D lerpPosition, float lerpRadius);
	void MulticastDecreaseCircle_Implementation(FVector2D lerpPosition, float lerpRadius);
	bool MulticastDecreaseCircle_Validate(FVector2D lerpPosition, float lerpRadius);

	UFUNCTION()
	void DecreaseCircle(FVector2D lerpPosition, float lerpRadius);

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void MulticastDecreaseCircleComplete();
	void MulticastDecreaseCircleComplete_Implementation();
	bool MulticastDecreaseCircleComplete_Validate();
};