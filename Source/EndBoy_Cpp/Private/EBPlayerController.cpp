// Fill out your copyright notice in the Description page of Project Settings.

#include "EBPlayerController.h"

AEBPlayerController::AEBPlayerController(const FObjectInitializer& ObjectInitializer) {
	
	bReplicates = true;

	inventory.Empty();

	static ConstructorHelpers::FClassFinder<UEBWidget_HUD>HUDWIDGET(TEXT("WidgetBlueprint'/Game/UI/UI_HUD.UI_HUD_C'"));
	if (HUDWIDGET.Succeeded()) {
		Widget_HUD = HUDWIDGET.Class;
	}
	else {
		EBLOG(Warning, TEXT("Main HUD not created in controller. plz restart game"));
	}

	isinventoryopen = false;
	isMapOpen = false;
	countInventorySlot = 10;
	targetPosition = FVector::ZeroVector;
}

void AEBPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEBPlayerController, inventory);
}

void AEBPlayerController::BeginPlay() {
	Super::BeginPlay();

	if (EB_HUD == nullptr) {
		if (IsLocalPlayerController()) {
			EB_HUD = Cast<UEBWidget_HUD>(CreateWidget(this, Widget_HUD));
			EB_HUD->AddToViewport();
			EB_HUD->ConstructHUD();
			EB_HUD->SetOwningPlayer(this);
		}
		else {
			EBLOG(Warning, TEXT("(%s)it is not localplayercontrolleer and not creating HUD"), Role == ROLE_Authority ? TEXT("Server") : TEXT("Client"));
		}
	}
	else {
		EBLOG(Warning, TEXT("(%s)Already playercontroller had HUD"), Role == ROLE_Authority ? TEXT("Server") : TEXT("Client"));
	}

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AEBPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Released, this, &AEBPlayerController::PressInventory);
	InputComponent->BindAction(TEXT("Map"), EInputEvent::IE_Released, this, &AEBPlayerController::PressMap);
}

void AEBPlayerController::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

void AEBPlayerController::Possess(APawn* aPawn) {
	Super::Possess(aPawn);
}

bool AEBPlayerController::ClientShowResultWindow_Validate(int rank) {
	return true;
}
void AEBPlayerController::ClientShowResultWindow_Implementation(int rank) {
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

	EB_HUD->ShowResultWindow(rank);
}

void AEBPlayerController::PressInventory() {
	if (isinventoryopen) {
		EB_HUD->inventory->SetVisibility(ESlateVisibility::Hidden);
		EB_HUD->inventory->clickwidget->SetVisibility(ESlateVisibility::Hidden);
		EB_HUD->weaponSlots[0]->SetVisibility(ESlateVisibility::Hidden);
		EB_HUD->weaponSlots[1]->SetVisibility(ESlateVisibility::Hidden);
		EB_HUD->map->SetVisibility(ESlateVisibility::Hidden);
		isMapOpen = false;
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		isinventoryopen = false;
	}
	else {
		EB_HUD->inventory->SetVisibility(ESlateVisibility::Visible);
		EB_HUD->inventory->BindInventory(inventory, countInventorySlot);
		EB_HUD->weaponSlots[0]->SetVisibility(ESlateVisibility::Visible);
		EB_HUD->weaponSlots[1]->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
		isinventoryopen = true;
	}
}

bool AEBPlayerController::PickupItem_Validate(AEBExpandable* item) {
	return true;
}
void AEBPlayerController::PickupItem_Implementation(AEBExpandable* item) {
	for (int i = 0; i < inventory.Num(); i++) {
		if (inventory[i]->ItemCategory == item->ItemCategory && inventory[i]->index == item->index) {
			int diff = inventory[i]->maxStack - inventory[i]->currentStack;
			if (diff != 0) {
				if (diff < item->currentStack) {
					inventory[i]->currentStack += diff;
					item->currentStack -= diff;
					if (inventory.Num() < countInventorySlot) {
						inventory.Add(item);
						ClientBindInventory();
						item->ServerPickupItem(ECollisionEnabled::NoCollision, this->GetPawn());
					}
					else {
						EBLOG(Warning, TEXT("Inventory is full"));
					}
					//PickupItem(item);
				}
				else {
					inventory[i]->currentStack += item->currentStack;
					item->currentStack = 0;
					for (int i = 0; i < inventory.Num(); i++) {
						ClientBindInventory();
					}
					item->ServerPickupItem(ECollisionEnabled::NoCollision, this->GetPawn());
				}
				return;
			}
		}
	}
	if (inventory.Num() < countInventorySlot) {
		inventory.Add(item);
		item->ServerPickupItem(ECollisionEnabled::NoCollision, this);
	}
	else {
		EBLOG(Warning, TEXT("Inventory is full"));
	}
}

void AEBPlayerController::OnRep_SetInventory() {
	EB_HUD->inventory->BindInventory(inventory, countInventorySlot);	
}

bool AEBPlayerController::ClientBindWeaponSlot_Validate(AEBWeapon* weapon, int slotnum) {
	return true;
}
void AEBPlayerController::ClientBindWeaponSlot_Implementation(AEBWeapon* weapon, int slotnum) {
	EB_HUD->weaponSlots[slotnum]->BindWeapon(weapon);
}

bool AEBPlayerController::ClientBindInventory_Validate() {
	return true;
}
void AEBPlayerController::ClientBindInventory_Implementation() {
	GetWorldTimerManager().SetTimer(UsingTimerHandle, this, &AEBPlayerController::OnRep_SetInventory, 0.1f, false, 0.1f);
}

void AEBPlayerController::PressMap() {
	if (isMapOpen) {
		EB_HUD->map->SetVisibility(ESlateVisibility::Hidden);
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		isMapOpen = false;
	}
	else {
		EB_HUD->inventory->SetVisibility(ESlateVisibility::Hidden);
		EB_HUD->inventory->clickwidget->SetVisibility(ESlateVisibility::Hidden);
		EB_HUD->weaponSlots[0]->SetVisibility(ESlateVisibility::Hidden);
		EB_HUD->weaponSlots[1]->SetVisibility(ESlateVisibility::Hidden);
		isinventoryopen = false;
		EB_HUD->map->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
		isMapOpen = true;
	}
}