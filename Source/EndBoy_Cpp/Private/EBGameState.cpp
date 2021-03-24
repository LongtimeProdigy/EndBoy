// Fill out your copyright notice in the Description page of Project Settings.

#include "EBGameState.h"

void AEBGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEBGameState, arrivedPlayerCount);
	DOREPLIFETIME(AEBGameState, currentCirclePosition);
	DOREPLIFETIME(AEBGameState, targetCirclePosition);
	DOREPLIFETIME(AEBGameState, currentCircleRadius);
	DOREPLIFETIME(AEBGameState, targetCircleRadius);
	DOREPLIFETIME(AEBGameState, phaseTime);
	DOREPLIFETIME(AEBGameState, shrinkTime);
	DOREPLIFETIME(AEBGameState, battleStartCountDown);
}

void AEBGameState::OnRep_SetArrivedPlayerCount() {
	Cast<AEBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->EB_HUD->players->textCount->SetText(FText::FromString(FString::FromInt(arrivedPlayerCount)));
}

bool AEBGameState::SettingCircle_Validate(FVector2D currentCirclePosition, FVector2D targetCirclePosition, float currentCircleRadius, float targetCircleRadius, int timephase, int timeshrink) {
	return true;
}
void AEBGameState::SettingCircle_Implementation(FVector2D circlePosition, FVector2D nextCirclePosition, float circleRadius, float nextCircleRadius, int timephase, int timeshrink){
	currentCirclePosition = circlePosition;
	targetCirclePosition = nextCirclePosition;
	currentCircleRadius = circleRadius;
	targetCircleRadius = nextCircleRadius;
	phaseTime = timephase;
	shrinkTime = timeshrink;
}

void AEBGameState::OnRep_SetBattleStartDelayTime() {
	EBLOG(Warning, TEXT("battleStartCountDown: %d"), battleStartCountDown);
}

bool AEBGameState::MulticastDecreaseCircle_Validate(FVector2D lerpPosition, float lerpRadius) {
	return true;
}
void AEBGameState::MulticastDecreaseCircle_Implementation(FVector2D lerpPosition, float lerpRadius) {
	timerDelegateCirclePhase.BindUFunction(this, FName("DecreaseCircle"), lerpPosition, lerpRadius);
	GetWorldTimerManager().SetTimer(timerCirclePhase, timerDelegateCirclePhase, 0.01f, true);
}

void AEBGameState::DecreaseCircle(FVector2D lerpPosition, float lerpRadius) {
	if (currentCircleRadius > targetCircleRadius) {
		currentCirclePosition += lerpPosition * 0.01f;
		currentCircleRadius += lerpRadius * 0.01f;
	}
	else {
		currentCirclePosition = targetCirclePosition;
		currentCircleRadius = targetCircleRadius;

		GetWorldTimerManager().ClearTimer(timerCirclePhase);
	}
}

bool AEBGameState::MulticastDecreaseCircleComplete_Validate() {
	return true;
}
void AEBGameState::MulticastDecreaseCircleComplete_Implementation() {
	currentCirclePosition = targetCirclePosition;
	currentCircleRadius = targetCircleRadius;

	GetWorldTimerManager().ClearTimer(timerCirclePhase);
}