// Fill out your copyright notice in the Description page of Project Settings.

#include "EBBullet.h"
#include "GameFramework/DamageType.h"
#include "DrawDebugHelpers.h"

// Sets default values
AEBBullet::AEBBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
	SetReplicateMovement(true);

	BulletDamage = 0;
	Instigator_Controller = nullptr;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Body->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = Body;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ShereCollision"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("NoCollision"));
	CollisionComponent->SetupAttachment(Body);
	CollisionComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEBBullet::OnShpereBeginOverlap);

	//Body->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 0.0f;
	ProjectileMovementComponent->MaxSpeed = 0.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->bInterpMovement = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->Velocity = FVector(0.0f, 0.0f, 0.0f);
	
}

// Called when the game starts or when spawned
void AEBBullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEBBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEBBullet::SetupBullet(FVector Loc, FRotator Rot, float BulletSpeed, FName ProfileName, AController* Instigator_Con) {
	CollisionComponent->SetCollisionProfileName(ProfileName);
	//CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//CollisionComponent->SetSimulatePhysics(true);
	//CollisionComponent->SetEnableGravity(false);
	SetActorLocation(Loc);
	SetActorRotation(Rot);
	//CollisionComponent->AddForce(GetActorForwardVector() * BulletSpeed);
	//ProjectileMovementComponent->MaxSpeed = BulletSpeed;
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * BulletSpeed;
	Instigator_Controller = Instigator_Con;
}

bool AEBBullet::OnShpereBeginOverlap_Validate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	return true;
}
void AEBBullet::OnShpereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	

	FHitResult OutHit;
	//TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	//UGameplayStatics::ApplyPointDamage(OtherActor, BulletDamage, this->GetActorForwardVector(), OutHit, Instigator_Controller, this, UDamageType::StaticClass());
	OtherActor->TakeDamage(BulletDamage, FDamageEvent::FDamageEvent(), Instigator_Controller, this);

	SetupBullet(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), 0.0f, FName("NoCollision"), Instigator_Controller);
	ProjectileMovementComponent->Velocity = FVector(0.0f, 0.0f, 0.0f);

	EBLOG(Warning, TEXT("(%s) Bullet overlaped and takedamage"), Role == ROLE_Authority ? TEXT("Server") : TEXT("Client"));
}