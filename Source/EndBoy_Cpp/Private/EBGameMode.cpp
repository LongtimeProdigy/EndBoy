// Fill out your copyright notice in the Description page of Project Settings.

#include "EBGameMode.h"
#include "EBCharacter.h"

AEBGameMode::AEBGameMode() {
	PlayerControllerClass = AEBPlayerController::StaticClass();
	//PlayerControllerClass = AEBPlayerController2::StaticClass();
	DefaultPawnClass = AEBCharacter::StaticClass();
	GameStateClass = AEBGameState::StaticClass();

	countPhase = 5;
	currentPhase = 0;
	currentCirclePosition = FVector2D::ZeroVector;
	targetCirclePosition = FVector2D::ZeroVector;

	int i = 0;
	for (i = 0; i < countPhase; i++) {
		circleRadius.Add(216375 - i * 50000);
	}
	circleRadius.Add(0);
	currentCircleRadius = circleRadius[currentPhase];

	for (i = 0; i < countPhase; i++) {
		// (temp) decrease time 1 min per phase
		timePhase.Add(3);
		// (temp) shrinktime not change about 2 sec
		shrinkTime.Add(5);
	}

	tempPositionforLerp = FVector2D::ZeroVector;
	tempRadiusforLerp = 0;

	currentGameState = EGameState::START;

	battleStartDelayTime = 10;
}

void AEBGameMode::BeginPlay() {
	Super::BeginPlay();
	EBGameState = GetGameState<AEBGameState>();
}

void AEBGameMode::PostLogin(APlayerController* newController) {
	Super::PostLogin(newController);

	//if (newController->GetPawn() != nullptr) {
	//	newController->GetPawn()->Destroy();
	//	SpawnCharacter(newController);
	//	UE_LOG(LogTemp, Warning, TEXT("%s is login! / Destroy"), *newController->GetName());
	//}
	//else
	//{
	//	SpawnCharacter(newController);
	//	UE_LOG(LogTemp, Warning, TEXT("%s is login! / Not Destroy"), *newController->GetName());
	//}

	LoginedActors.Add(newController);
	ArrivedActors.Add(newController);

	GetGameState<AEBGameState>()->arrivedPlayerCount = ArrivedActors.Num();

	EBLOG(Warning, TEXT("gamemode postlogin!"));

	// ... (temp) BATTLE START WITH TWO PLAYER
	if (ArrivedActors.Num() >= 2) {
		GetWorldTimerManager().SetTimer(timerBattleStart, this, &AEBGameMode::BattleStartCountDown, 1, true, 0);
	}
}

bool AEBGameMode::SpawnCharacter_Validate(APlayerController* newController) {
	return true;
}
void AEBGameMode::SpawnCharacter_Implementation(APlayerController* newController) {
	if (newController != nullptr) {
		FVector2D SpawnLoc2D = FMath::RandPointInCircle(600.0f);
		FActorSpawnParameters spawninfo;
		auto pawn = GetWorld()->SpawnActor<AEBCharacter>(AEBCharacter::StaticClass(), FVector(SpawnLoc2D, 150.0f), FRotator::ZeroRotator, spawninfo);
		LoginedActors.AddUnique(pawn);

		newController->Possess(pawn);
		UE_LOG(LogTemp, Warning, TEXT("%s is possessed"), *newController->GetName());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("%s is not possessed"), *newController->GetName());
	}
}

void AEBGameMode::PlayerDead(APlayerController* actor) {
	ArrivedActors.Remove(actor);
	DeadedActors.Add(actor);

	EBGameState->arrivedPlayerCount = ArrivedActors.Num();

	if (ArrivedActors.Num() <= 1) {
		currentGameState = EGameState::FINISH;
		electricfield->ServerEndElectricField();
		int i = 0;
		for (i = 0; i < ArrivedActors.Num(); i++) {
			if (ArrivedActors[i] != nullptr) {
				Cast<AEBPlayerController>(ArrivedActors[i])->ClientShowResultWindow(1);
			}
			else {
				EBLOG(Warning, TEXT("SomeError======="));
			}
		}
	}
}

void AEBGameMode::BattleStartCountDown() {
	if (battleStartDelayTime > 0) {
		battleStartDelayTime--;
		EBGameState->battleStartCountDown = battleStartDelayTime;
	}
	else {
		GetWorldTimerManager().ClearTimer(timerBattleStart);
		BattleStart();
	}
}

void AEBGameMode::BattleStart() {
	currentGameState = EGameState::BATTLE;
	SettingCircle();
	FActorSpawnParameters spawninfo;
	electricfield = GetWorld()->SpawnActor<AEBElectricField>(FVector::ZeroVector, FRotator::ZeroRotator, spawninfo);
	electricfield->ServerStartElectricField();
}

void AEBGameMode::SettingCircle() {
	if (currentPhase < countPhase) {
		//currentCirclePosition resetting -- currentphase 0:: targetcircleposition is zero
		currentCirclePosition = targetCirclePosition;

		int x;
		int y;
		if (circleRadius[currentPhase] - circleRadius[currentPhase + 1] > 0) {
			float tempRadius = FMath::RandRange(0.0f, circleRadius[currentPhase] - circleRadius[currentPhase + 1]);
			float tempDegree = FMath::RandRange(0, 360);
			x = tempRadius * FMath::Sin(tempDegree);
			y = tempRadius * FMath::Cos(tempDegree);
		}
		else {
			x = 0;
			y = 0;
		}
		
		targetCirclePosition = FVector2D(x, y) + currentCirclePosition;

		GetGameState<AEBGameState>()->SettingCircle(currentCirclePosition, targetCirclePosition, circleRadius[currentPhase], circleRadius[currentPhase + 1], timePhase[currentPhase], shrinkTime[currentPhase]);

		if (currentPhase < countPhase) {
			tempPositionforLerp = (targetCirclePosition - currentCirclePosition) / shrinkTime[currentPhase];
			tempRadiusforLerp = (circleRadius[currentPhase + 1] - circleRadius[currentPhase]) / shrinkTime[currentPhase];

			timerDelegateCirclePhase.BindUFunction(EBGameState, FName("MulticastDecreaseCircle"), tempPositionforLerp, tempRadiusforLerp);

			GetWorldTimerManager().SetTimer(timerCirclePhase1, timerDelegateCirclePhase, timePhase[currentPhase], false);
			GetWorldTimerManager().SetTimer(timerCirclePhase2, this, &AEBGameMode::DecreaseMagneticField, timePhase[currentPhase] + shrinkTime[currentPhase], false);
		}
		else {
			EBLOG(Warning, TEXT("Phase End"));
		}
	}
	else {
		EBLOG(Warning, TEXT("Phase is already max, game will not  exit. plz check this"));
	}
}

void AEBGameMode::DecreaseMagneticField() {
	GetWorldTimerManager().ClearTimer(timerCirclePhase1);
	GetWorldTimerManager().ClearTimer(timerCirclePhase2);

	EBGameState->MulticastDecreaseCircleComplete();

	currentPhase++;
	SettingCircle();
}

int AEBGameMode::GetArrivedPlayerCount() {
	return ArrivedActors.Num();
}