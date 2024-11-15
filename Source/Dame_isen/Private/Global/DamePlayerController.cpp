// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/DamePlayerController.h"
#include "GamePlay/DamePion.h"

void ADamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(GameplayMappingContext, 0);
	}

}

void ADamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Select, ETriggerEvent::Started, this, &ADamePlayerController::TrySelect);
	}

}

void ADamePlayerController::TrySelect()
{
	UE_LOG(LogTemp, Warning, TEXT("Ca marche pas si mal !!"));

	//TODO : R�cup�rer la position de la souris
	FVector MouseLocation, MouseForward;
	DeprojectMousePositionToWorld(MouseLocation, MouseForward);
	
	
	FHitResult HitResult;
	FVector EndRay = MouseLocation + MouseForward * 1000;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this->GetPawn());  


	//TODO : V�rifier ce qu'il y a sous la souirs (LineTrace)
	if (GetWorld()->LineTraceSingleByChannel(HitResult, MouseLocation, EndRay, ECC_Visibility,Params)) {
		AActor* target = HitResult.GetActor();
		if (target->Implements<UISelectable>()) {
			if (TScriptInterface<IISelectable>(target) != Selected) {
				
				Selected = TScriptInterface<IISelectable>(target);
				UE_LOG(LogTemp, Warning, TEXT("J'ai touche un objet selectionnable !"));
				Selected->Select();
			}
		}
		
		//ADamePion* pion = Cast<ADamePion>(HitResult.GetActor());

		//if (pion != nullptr) {
		//	UE_LOG(LogTemp, Warning, TEXT("J'ai touche un pion !"));
		//}
	}

	//TODO : SI c'est un pion ... v�rifier si le pion correspond au joueur actif puis v�rifier les d�placements

}
