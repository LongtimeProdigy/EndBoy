// Fill out your copyright notice in the Description page of Project Settings.

#include "EBCharacter.h"
#include "DrawDebugHelpers.h"
#include "EBGameMode.h"

//setup replication
void AEBCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEBCharacter, IsProne);
	DOREPLIFETIME(AEBCharacter, IsCrouch);
	DOREPLIFETIME(AEBCharacter, C_Pose);
	DOREPLIFETIME(AEBCharacter, PawnController);
	DOREPLIFETIME(AEBCharacter, IsEquiped);
	DOREPLIFETIME(AEBCharacter, CurrentWeapon);
	DOREPLIFETIME(AEBCharacter, HP);
	DOREPLIFETIME(AEBCharacter, ControllerPitch);
	DOREPLIFETIME(AEBCharacter, IsAming);
	DOREPLIFETIME(AEBCharacter, IsSprint);
	DOREPLIFETIME(AEBCharacter, currentItem);
}

// Sets default values
AEBCharacter::AEBCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	IsProne = false;
	IsCrouch = false;
	IsSprint = false;
	IsEquiped = false;
	IsAming = false;
	IsFreeviewed = false;
	C_Pose = Character_Pose::STAND;
	maxHP = 100.0f;
	HP = maxHP;

	Checked_Item = nullptr;
	currentItem = nullptr;

	//Make SpringArm, Camera Component and setup them
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
	

	//Call MeshAsset and setup that
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MESH_BODY(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (MESH_BODY.Succeeded()) {
		GetMesh()->SetSkeletalMesh(MESH_BODY.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionProfileName("Pawn");

	SpringArm->SetupAttachment(GetMesh());
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeLocation(FVector(0, 0 ,160));
	SpringArm->SetRelativeRotation(FRotator(0, 0, 90));
	
	//setup mesh animation
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>MESH_ANIM(TEXT("AnimBlueprint'/Game/Mannequin/Character/Animation/BPAnim_EBCharacter.BPAnim_EBCharacter_C'"));
	if (MESH_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(MESH_ANIM.Class);
	}

	// get weapon chane montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage>RELOADMONTAGE(TEXT("AnimMontage'/Game/Mannequin/Character/Animation/Reload_Rifle_Hip_Montage.Reload_Rifle_Hip_Montage'"));
	if (RELOADMONTAGE.Succeeded()) {
		montageWeaponChange = RELOADMONTAGE.Object;
	}

	//setup rotation
	bUseControllerRotationYaw = true;
	SpringArm->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AEBCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEBCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	PawnController = Cast<AEBPlayerController>(NewController);

}

// Called every frame
void AEBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PawnController != nullptr && PawnController->EB_HUD != nullptr) {
		if (PawnController->IsLocalController()) {
			//무기 탄창 UI설정
			if (CurrentWeapon) {
				PawnController->EB_HUD->WeaponInfo->BindWeapon(CurrentWeapon);				
			}
			else {
				PawnController->EB_HUD->WeaponInfo->BindWeapon(nullptr);
			}
			// 조준 및 점프 중이 아니라면
			if (!IsAming && !GetCharacterMovement()->IsFalling()) {
				// 자기 앞에 Ray하여 아이템들을 검출

				FHitResult Hit;
				FCollisionQueryParams collisionParams(NAME_None, false, this);
				FVector V_start;
				FVector V_end;

				V_start = GetActorLocation();
				V_end = V_start + Camera->GetForwardVector() * 200;

				if (GetWorld()->LineTraceSingleByChannel(Hit, V_start, V_end, ECC_Visibility, collisionParams)) {
					DrawDebugLine(GetWorld(), V_start, V_end, FColor::Green, false, 5, 0, 1);
					if (Cast<AEBItem>(Hit.Actor) != nullptr) {
						Checked_Item = Cast<AEBItem>(Hit.Actor);
						PawnController->EB_HUD->PickupMassamge->BindItem(Checked_Item, "Press F to pickup ");
					}
					else {
						Checked_Item = nullptr;
					}
				}
				else {
					Checked_Item = nullptr;
				}
			}
			else {
				Checked_Item = nullptr;
			}

			if (currentItem) {
				PawnController->EB_HUD->PickupMassamge->BindItem(currentItem, "Press F to cancle ");
			}
			else {
				PawnController->EB_HUD->PickupMassamge->BindItem(Checked_Item, "Press F to cancle ");
			}

			/*if (Checked_Item) {
				EBLOG(Warning, TEXT("%s"), *Checked_Item->GetName());
			}*/
		}
	}

	//에임 오프셋 세팅
	if (PawnController) {
		if (PawnController->IsLocalController()) {
			ServerSetPitch(GetControlRotation().Pitch > 180.0f ? 360.0f - GetControlRotation().Pitch : GetControlRotation().Pitch*(-1.0f));
		}
	}
}

bool AEBCharacter::ServerSetPitch_Validate(float Pitch) {
	return true;
}
void AEBCharacter::ServerSetPitch_Implementation(float Pitch) {
	ControllerPitch = Pitch;
}

// Called to bind functionality to input
void AEBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AEBCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AEBCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AEBCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AEBCharacter::LookUp);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AEBCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AEBCharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AEBCharacter::ServerCrouching);
	PlayerInputComponent->BindAction(TEXT("Prone"), EInputEvent::IE_Pressed, this, &AEBCharacter::ServerProning);
	PlayerInputComponent->BindAction(TEXT("Interactive"), EInputEvent::IE_Pressed, this, &AEBCharacter::Interactive);
	PlayerInputComponent->BindAction(TEXT("Drop"), EInputEvent::IE_Pressed, this, &AEBCharacter::ServerDropItem);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AEBCharacter::ServerDoFireStart);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &AEBCharacter::ServerDoFireStop);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AEBCharacter::ServerReload);
	PlayerInputComponent->BindAction(TEXT("Zoom"), EInputEvent::IE_Pressed, this, &AEBCharacter::ClickRightMouseButton);
	PlayerInputComponent->BindAction(TEXT("Freeview"), EInputEvent::IE_Pressed, this, &AEBCharacter::Freeview);
	PlayerInputComponent->BindAction(TEXT("Freeview"), EInputEvent::IE_Released, this, &AEBCharacter::Freeview);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AEBCharacter::ServerSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AEBCharacter::ServerSprint);
	PlayerInputComponent->BindAction(TEXT("ChangeShootMode"), EInputEvent::IE_Pressed, this, &AEBCharacter::ServerChangeShootMode);
	PlayerInputComponent->BindAction(TEXT("Slot1"), EInputEvent::IE_Pressed, this, &AEBCharacter::ServerWeaponChange<0>);
	PlayerInputComponent->BindAction(TEXT("Slot2"), EInputEvent::IE_Pressed, this, &AEBCharacter::ServerWeaponChange<1>);
	//PlayerInputComponent->BindAction(TEXT("WeaponInteraction"), EInputEvent::IE_Pressed, this, &AEBCharacter::ServerWeaponInteraction);
}
void AEBCharacter::MoveForward(float NewAxisValue) {
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}
void AEBCharacter::MoveRight(float NewAxisValue) {
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}
void AEBCharacter::Turn(float NewAxisValue) {
	AddControllerYawInput(NewAxisValue);
}
void AEBCharacter::LookUp(float NewAxisValue) {
	AddControllerPitchInput(NewAxisValue);
}

void AEBCharacter::Interactive() {
	if (currentItem) {
		ServerCancleItem();
	}
	else {
		if (Checked_Item) {
			ServerPickupItem(Checked_Item);
		}
		else {
			EBLOG(Warning, TEXT("Nothing to pickup"));
		}
		EBLOG(Warning, TEXT("%s"), Checked_Item ? *Checked_Item->GetName() : TEXT("NONE"));
	}
}

bool AEBCharacter::ServerPickupItem_Validate(AEBItem* Itemtopickup){
	return Itemtopickup != nullptr;
}
void AEBCharacter::ServerPickupItem_Implementation(AEBItem* Itemtopickup) {
	if (Itemtopickup) {
		switch (Itemtopickup->ItemCategory)
		{
		case ItemCategory::NOTHING:
			break;
		case ItemCategory::WEAPON:
			Itemtopickup->ServerPickupItem(ECollisionEnabled::NoCollision, this);
			if (!weapon1) {
				weapon1 = Cast<AEBWeapon>(Itemtopickup);
				weapon1->slotNum = 0;
				weapon1->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("back_r_weapon"));
				PawnController->ClientBindWeaponSlot(weapon1, 0);
				if (!CurrentWeapon) {
					weapon1->ServerPickupItem(ECollisionEnabled::NoCollision, this);
					CurrentWeapon = weapon1;
					//IsEquiped = true;
				}
			}
			else if (!weapon2) {
				weapon2 = Cast<AEBWeapon>(Itemtopickup);
				weapon2->slotNum = 1;
				weapon2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("back_l_weapon"));
				PawnController->ClientBindWeaponSlot(weapon2, 1);
				if (!CurrentWeapon) {
					weapon2->ServerPickupItem(ECollisionEnabled::NoCollision, this);
					CurrentWeapon = weapon2;
					//IsEquiped = true;
				}
			}
			else {
				EBLOG(Warning, TEXT("WeaponSlot is full"));
				return;
			}
			break;
		case ItemCategory::EXPENDABLE:
			if (PawnController) {
				PawnController->PickupItem(Cast<AEBExpandable>(Itemtopickup));
			}
			else {
				return;
			}
			break;
		default:
			break;
		}

		Itemtopickup->Owner_Controller = PawnController;
		Itemtopickup->SetOwner(PawnController);
	}
	else {
		EBLOG(Warning, TEXT("Item to pickup is NULL"));
	}
}

bool AEBCharacter::ServerDropItem_Validate() {
	return true;
}
void AEBCharacter::ServerDropItem_Implementation() {
	if (CurrentWeapon) {
		if (IsAming) {
			ClientUnAming();
		}
		CurrentWeapon->Owner_Controller = nullptr;
		CurrentWeapon->ServerDropItem();		
		if (CurrentWeapon == weapon1) {
			weapon1 = nullptr;
			PawnController->ClientBindWeaponSlot(nullptr, 0);
		}
		else {
			weapon2 = nullptr;
			PawnController->ClientBindWeaponSlot(nullptr, 1);
		}
		PawnController->ClientBindWeaponSlot(nullptr, CurrentWeapon->slotNum);
		CurrentWeapon = nullptr;
		//IsEquiped = false;
	}
	else {
		EBLOG(Warning, TEXT("Nothing to drop"));
	}
}

bool AEBCharacter::ServerDoFireStart_Validate() {
	return true;
}
void AEBCharacter::ServerDoFireStart_Implementation() {
	if (CurrentWeapon) {
		if (currentItem) {
			ServerCancleItem();
		}
		CurrentWeapon->DoFireStart();
	}
	else {
		EBLOG(Warning, TEXT("No Weapon to shoot / Excute DoFireStop"));
	}
}

bool AEBCharacter::ServerDoFireStop_Validate() {
	return true;
}
void AEBCharacter::ServerDoFireStop_Implementation() {
	if (CurrentWeapon) {
		CurrentWeapon->DoFireStop();
	}
	else {
		EBLOG(Warning, TEXT("No Weapon to shoot"));
	}
}

bool AEBCharacter::ServerCrouching_Validate() {
	return true;
}
void AEBCharacter::ServerCrouching_Implementation() {
	if (!IsCrouch) {
		MulticastChange_Pose(Character_Pose::CROUCH);
	}
	else {
		MulticastChange_Pose(Character_Pose::STAND);
	}
}

bool AEBCharacter::ServerProning_Validate() {
	return true;
}
void AEBCharacter::ServerProning_Implementation() {
	if (!IsProne) {
		MulticastChange_Pose(Character_Pose::PRONE);
	}
	else {
		MulticastChange_Pose(Character_Pose::STAND);
	}
}

bool AEBCharacter::MulticastChange_Pose_Validate(Character_Pose To_Pose) {
	return true;
}
void AEBCharacter::MulticastChange_Pose_Implementation(Character_Pose To_Pose) {
	switch (To_Pose)
	{
	case Character_Pose::CROUCH:
			C_Pose = Character_Pose::CROUCH;
			IsProne = false;
			IsCrouch = true;
			GetCharacterMovement()->MaxWalkSpeed = 300;
			
		break;
	case Character_Pose::PRONE:
			C_Pose = Character_Pose::PRONE;
			IsCrouch = false;
			IsProne = true;
			GetCharacterMovement()->MaxWalkSpeed = 120;
		break;
	default:
		C_Pose = Character_Pose::STAND;
		IsCrouch = false;
		IsProne = false;
		GetCharacterMovement()->MaxWalkSpeed = 600;
		break;
	}
}

bool AEBCharacter::ServerReload_Validate() {
	return true;
}
void AEBCharacter::ServerReload_Implementation() {
	if (CurrentWeapon) {
		if (CurrentWeapon->ReloadAnim) {
			if (CurrentWeapon->canReload && CurrentWeapon->currentState == EState::IDLE) {
				if (CurrentWeapon->CurrentAmmo != 0) {
					if (currentItem) {
						ServerCancleItem();
					}
					if (IsAming) {
						ClientUnAming();
					}
					CurrentWeapon->MakeUnUsableWeapon();
					MulticastPlayMontage(CurrentWeapon->ReloadAnim);
					float Duration = CurrentWeapon->ReloadAnim->GetPlayLength();
					GetWorldTimerManager().SetTimer(reloadTimerHandle, this, &AEBCharacter::ReloadComplete, Duration);
				}
				else {
					EBLOG(Warning, TEXT("Already ammo is full"));
				}
			}
			else {
				EBLOG(Warning, TEXT("Already reloading or shooting"));
			}
		}
		else {
			EBLOG(Warning, TEXT("ReloadAnim is NULL"));
		}
	}
	else {
		EBLOG(Warning, TEXT("%s don't equip weapon"), *GetName());
	}
}
void AEBCharacter::ReloadComplete() {
	GetWorldTimerManager().ClearTimer(reloadTimerHandle);
	CurrentWeapon->ReloadComplete();
}

bool AEBCharacter::MulticastPlayMontage_Validate(UAnimMontage* Montage){
	return true;
}
void AEBCharacter::MulticastPlayMontage_Implementation(UAnimMontage* Montage){
	if (currentMontage) {
		ServerStopMontage();
	}
	PlayAnimMontage(Montage);
	currentMontage = Montage;
}

bool AEBCharacter::ServerStopMontage_Validate() {
	return true;
}
void AEBCharacter::ServerStopMontage_Implementation() {
	MulticastStopMontage();
}

bool AEBCharacter::MulticastStopMontage_Validate() {
	return true;
}
void AEBCharacter::MulticastStopMontage_Implementation() {
	StopAnimMontage(currentMontage);
	currentMontage = nullptr;
}

//apply damage on server from Bullet.h
float AEBCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	const float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (Role == ROLE_Authority) {
		HP = HP - FinalDamage;
		OnRep_SetHP();

		if (HP <= 0.0f) {
			HP = 0.0f;
			C_Pose = Character_Pose::DEAD;

			if (currentItem) {
				ServerCancleItem();
			}

			ClientDead(Cast<AEBGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetArrivedPlayerCount());
			MulticastDead(DamageCauser->GetActorForwardVector());

			Cast<AEBGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->PlayerDead(PawnController);
		}
		//EBLOG(Warning, TEXT("DamageCauser name: %s, forwardvector: %s"), *DamageCauser->GetName(), *DamageCauser->GetActorForwardVector().ToString());
	}
	return FinalDamage;
}

bool AEBCharacter::MulticastDead_Validate(FVector directionvector) {
	return true;
}
void AEBCharacter::MulticastDead_Implementation(FVector directionvector) {
	GetMesh()->SetSimulatePhysics(true);
	GetCharacterMovement()->Velocity = directionvector * 1000;
}
bool AEBCharacter::ClientDead_Validate(int rank) {
	return true;
}
void AEBCharacter::ClientDead_Implementation(int rank) {
	if (PawnController) {
		PawnController->ClientShowResultWindow(rank);
		PawnController->ClientSetRotation(FRotator::ZeroRotator);
	}
}

void AEBCharacter::ClickRightMouseButton() {
	if (CurrentWeapon && !GetCharacterMovement()->IsFalling()) {
		if (!IsAming) {
			ClientAming();
		}
		else {
			ClientUnAming();
		}
	}
}

bool AEBCharacter::ClientAming_Validate() {
	return true;
}
void AEBCharacter::ClientAming_Implementation() {
	if (CurrentWeapon) {
		if (currentItem) {
			ServerCancleItem();
		}
		if (!IsAming) {
			CurrentWeapon->Zooming(PawnController);
			ServerSetIsAming(true);
		}
	}
}
bool AEBCharacter::ClientUnAming_Validate() {
	return true;
}
void AEBCharacter::ClientUnAming_Implementation() {
	if (CurrentWeapon) {
		if (IsAming) {
			PawnController->SetViewTargetWithBlend(this);
			ServerSetIsAming(false);
		}
	}
}
bool AEBCharacter::ServerSetIsAming_Validate(bool _IsAming) {
	return true;
}
void AEBCharacter::ServerSetIsAming_Implementation(bool _IsAming) {
	IsAming = _IsAming;
}

void AEBCharacter::Freeview() {
	if(!IsFreeviewed){
		bUseControllerRotationYaw = false;
		IsFreeviewed = true;
	}
	else {
		bUseControllerRotationYaw = true;
		IsFreeviewed = false;
	}
}

bool AEBCharacter::ServerSprint_Validate() {
	return true;
}
void AEBCharacter::ServerSprint_Implementation() {
	if (!IsSprint) {
		if (currentItem) {
			ServerCancleItem();
		}
		IsSprint = true;
		
		MulticastSetSpeed(GetCharacterMovement()->MaxWalkSpeed *= 1.3f);
	}
	else {
		IsSprint = false;
		
		MulticastSetSpeed(GetCharacterMovement()->MaxWalkSpeed /= 1.3f);
	}

	EBLOG(Warning, TEXT("MoveSpeed : %f"), GetCharacterMovement()->MaxWalkSpeed);
}

void AEBCharacter::OnRep_SetHP() {
	if (IsLocallyControlled()) {
		PawnController->EB_HUD->HPBar->BindHPBar(HP);
	}
}

//bool AEBCharacter::Server_Healing_Validate(float amount) {
//	return true;
//}
//void AEBCharacter::Server_Healing_Implementation(float amount) {
//	HP += amount;
//	HP = FMath::Clamp(HP, 0.0f, 100.0f);
//}

bool AEBCharacter::ServerChangeShootMode_Validate() {
	return true;
}
void AEBCharacter::ServerChangeShootMode_Implementation() {
	if (Role = ROLE_Authority) {
		if (CurrentWeapon) {
			CurrentWeapon->ChangeShootMode();
		}
	}
	else {
		EBLOG(Warning, TEXT("Not Shoot Cuz not Server"));
	}
}

bool AEBCharacter::ServerWeaponChange_Validate(int slotnum) {
	return true;
}
void AEBCharacter::ServerWeaponChange_Implementation(int slotnum) {
	EBLOG(Warning, TEXT("Slot Pressed : %d"), slotnum);

	if (currentItem) {
		ServerCancleItem();
	}

	switch (slotnum)
	{
	case 0:
		if (weapon1 && CurrentWeapon != weapon1) {
			MulticastPlayMontage(montageWeaponChange);
			float montageLength = montageWeaponChange->GetPlayLength();
			weaponchangeTimerDelegate.BindUFunction(this, FName("WeaponChangeComplete"), slotnum);
			GetWorldTimerManager().SetTimer(reloadTimerHandle, weaponchangeTimerDelegate, montageLength, false);
		}
		break;
	case 1:
		if (weapon2 && CurrentWeapon != weapon2) {
			MulticastPlayMontage(montageWeaponChange);
			float montageLength = montageWeaponChange->GetPlayLength();
			weaponchangeTimerDelegate.BindUFunction(this, FName("WeaponChangeComplete"), slotnum);
			GetWorldTimerManager().SetTimer(reloadTimerHandle, weaponchangeTimerDelegate, montageLength, false);
		}
		break;
	default:
		break;
	}

	
}
void AEBCharacter::WeaponChangeComplete(int slotnum) {
	switch (slotnum)
	{
	case 0:
		if (CurrentWeapon)
		{
			CurrentWeapon->ServerUnequip(1);
		}
		weapon1->ServerPickupItem(ECollisionEnabled::NoCollision, this);
		CurrentWeapon = weapon1;
		break;
	case 1:
		if (CurrentWeapon)
		{
			CurrentWeapon->ServerUnequip(0);
		}
		weapon2->ServerPickupItem(ECollisionEnabled::NoCollision, this);
		CurrentWeapon = weapon2;
		break;
	default:
		break;
	}
}

//bool AEBCharacter::ServerWeaponInteraction_Validate() {
//	return true;
//}
//void AEBCharacter::ServerWeaponInteraction_Implementation() {
//	if (CurrentWeapon) {
//		CurrentWeapon->ServerLevelUp();
//	}
//	else {
//		EBLOG(Warning, TEXT("There are not equiped weapon to levelup"));
//	}
//}

bool AEBCharacter::ServerCancleItem_Validate() {
	return true;
}
void AEBCharacter::ServerCancleItem_Implementation() {
	EBLOG(Warning, TEXT("Item Cancled : %s"), *currentItem->GetName());
	GetWorldTimerManager().ClearTimer(currentItem->useTimer);
	currentItem = nullptr;
	MulticastSetSpeed(600);
	if (currentMontage) {
		ServerStopMontage();
	}
}

//bool AEBCharacter::ServerBeginOverlapCircle_Validate(float damage, AActor* causor) {
//	return true;
//}
//void AEBCharacter::ServerBeginOverlapCircle_Implementation(float damage, AActor* causor) {
//	delegateCircleDamage.BindUFunction(this, FName("BeginOverlapCircle"), damage, causor);
//	GetWorldTimerManager().SetTimer(timerCircleDamage, delegateCircleDamage, 1, true, 1);
//}
//
//void AEBCharacter::BeginOverlapCircle(float damage) {
//	if (Role == ROLE_Authority) {
//		HP = FMath::Clamp(HP - damage, 0.0f, maxHP);
//		OnRep_SetHP();
//
//		if (HP <= 0.0f) {
//			HP = 0.0f;
//			C_Pose = Character_Pose::DEAD;
//
//			if (currentItem) {
//				ServerCancleItem();
//			}
//
//			ClientDead();
//			MulticastDead(GetActorForwardVector());
//
//			Cast<AEBGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->PlayerDead(PawnController);
//		}
//		//EBLOG(Warning, TEXT("DamageCauser name: %s, forwardvector: %s"), *DamageCauser->GetName(), *DamageCauser->GetActorForwardVector().ToString());
//	}
//	TakeDamage(damage, FDamageEvent::FDamageEvent(), PawnController, this)
//}
//
//bool AEBCharacter::ServerEndOverlapCircle_Validate() {
//	return true;
//}
//void AEBCharacter::ServerEndOverlapCircle_Implementation() {
//	GetWorldTimerManager().ClearTimer(timerCircleDamage);
//}

bool AEBCharacter::MulticastSetSpeed_Validate(float speed){
	return true;
}
void AEBCharacter::MulticastSetSpeed_Implementation(float speed) {
	GetCharacterMovement()->MaxWalkSpeed = speed;
}