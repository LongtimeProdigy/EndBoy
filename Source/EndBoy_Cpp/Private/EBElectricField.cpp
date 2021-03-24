// Fill out your copyright notice in the Description page of Project Settings.

#include "EBElectricField.h"

// Sets default values
AEBElectricField::AEBElectricField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	SetReplicates(true);
	SetReplicateMovement(true);
	NetCullDistanceSquared = 999999999999;

	body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	RootComponent = body;

	collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	collision->SetupAttachment(body);
	if (Role == ROLE_Authority) {
		collision->SetCollisionProfileName("Circle");
		collision->SetCapsuleRadius(50);
		collision->SetCapsuleHalfHeight(55);
		collision->SetEnableGravity(false);
		collision->OnComponentBeginOverlap.AddDynamic(this, &AEBElectricField::OnCapsuleBeginOverlap);
		collision->OnComponentEndOverlap.AddDynamic(this, &AEBElectricField::OnCapsuleEndOverlap);
	}
	else {
		collision->SetCollisionProfileName("NoCollision");
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MESH_BODY(TEXT("StaticMesh'/Game/EletricField/SM_EletricCylinder.SM_EletricCylinder'"));
	if (MESH_BODY.Succeeded()) {
		body->SetStaticMesh(MESH_BODY.Object);
		body->SetCollisionProfileName("NoCollision");
		body->SetEnableGravity(false);
	}

	damage = 10;
	damageCycleTime = 1;
}

void AEBElectricField::BeginPlay() {
	Super::BeginPlay();
	gameState = Cast<AEBGameState>(UGameplayStatics::GetGameState(GetWorld()));
}

void AEBElectricField::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (Role == ROLE_Authority) {
		circlePosition = FVector(gameState->currentCirclePosition.X, gameState->currentCirclePosition.Y, 0);
		SetActorLocation(circlePosition, false, nullptr);
	}

	circleRadius = gameState->currentCircleRadius;
	FVector tempScale = FVector(circleRadius / 50, circleRadius / 50, 1000);
	SetActorScale3D(tempScale);

	EBLOG(Warning, TEXT("radius: %f"), collision->GetScaledCapsuleRadius());
}

bool AEBElectricField::ServerStartElectricField_Validate() {
	return true;
}
void AEBElectricField::ServerStartElectricField_Implementation() {
	GetWorldTimerManager().SetTimer(timerCircleDamage, this, &AEBElectricField::SettingElectricField, damageCycleTime, true);
}

void AEBElectricField::SettingElectricField() {
	for (int i = 0; i < outsideActors.Num(); i++) {
		AEBCharacter* actor = outsideActors[i];
		actor->TakeDamage(damage, FDamageEvent::FDamageEvent(), actor->PawnController, this);
	}
}

bool AEBElectricField::ServerEndElectricField_Validate() {
	return true;
}
void AEBElectricField::ServerEndElectricField_Implementation() {
	GetWorldTimerManager().ClearTimer(timerCircleDamage);
}

void AEBElectricField::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (Role == ROLE_Authority) {
		AEBCharacter* actor = Cast<AEBCharacter>(OtherActor);
		//if (actor) {
			if (outsideActors.Contains(actor)) {
				outsideActors.Remove(actor);
			}
			EBLOG(Warning, TEXT("Actor is overlaped: %s"), *actor->GetName());
		//}
	}
}

void AEBElectricField::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (Role == ROLE_Authority) {
		AEBCharacter* actor = Cast<AEBCharacter>(OtherActor);
		//if (actor) {
			outsideActors.AddUnique(actor);
			EBLOG(Warning, TEXT("Actor is overlap ended: %s"), *actor->GetName());
		//}
	}
}