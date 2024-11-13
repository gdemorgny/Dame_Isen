// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/DamePion.h"

#include "GamePlay/DameCase.h"
#include "Slate/SGameLayerManager.h"

// Sets default values
ADamePion::ADamePion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void ADamePion::Die()
{
	UE_LOG(LogTemp,Warning,TEXT("Try Removing Pawn"))
	for(int i =0;i<Board->DameGM->PlayerPawns[(PlayerId == 1)?0:1].Pawns.Num();i++)
	{
		if(Board->DameGM->PlayerPawns[(PlayerId == 1)?0:1].Pawns[i] == this)
		{
			Board->DameGM->PlayerPawns[(PlayerId == 1)?0:1].Pawns.RemoveAt(i);
			UE_LOG(LogTemp,Warning,TEXT("Remove Pawn number %d"),i)
			break;
		}
	}
	Board->Cells[CellNumber]->PawnInCell = nullptr;
	Board->UpdatePlayerValueInCell(CellNumber);
	Destroy();
}

void ADamePion::BecameAQueen()
{
	if(Board->DameGameState.ActivePlayer == PlayerId)
	{
		if((PlayerId	==1 && CellNumber >= 56)|| (PlayerId	== -1 && CellNumber < 8 ))
		{
			IsQueen = true;
			FVector newLoc = GetActorLocation();
			newLoc.Z +=50.f;
			SetActorLocation(newLoc);
		} 
	}
}

// Called when the game starts or when spawned
void ADamePion::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADamePion::Select()
{
	if(Board->DameGM->ActualPlayer == PlayerId)
	{
		ChangeMaterial(true);
		//TODO : Informer le board qu'un pion est sélectionné et qu'il doit déterminer les chemins possibles.
		Board->NewPawnSelect(this);
	}
	
	
}

void ADamePion::Unselect()
{
	for(int i=0;i<Board->CellValidMoves.Num();i++)
	{
		Board->CellHighlight(Board->CellValidMoves[i].Cells.Last());
	}
	
	ChangeMaterial(false);
}

void ADamePion::StartMoveSequence(const TArray<FVector>& InMoveSequence, TArray<ADamePion*> InOpponentToDestroy)
{
	if (InMoveSequence.Num() == 0) return; // S'assurer qu'il y a une séquence

	MoveSequence = InMoveSequence;
	OpponentsToDestroy = InOpponentToDestroy;
	
	

	// Initialisation des positions pour le premier mouvement
	MoveProgress = 0.0f;
	StartPosition = GetActorLocation();
	TargetPosition = MoveSequence[0];
	MoveSequence.RemoveAt(0);

	// Lancer le premier déplacement
	MoveToNextPosition();
}

void ADamePion::MoveToNextPosition()
{
	
	GetWorldTimerManager().ClearTimer(MovementTimer);
	MoveProgress = 0.0f;

	
	GetWorldTimerManager().SetTimer(MovementTimer, this, &ADamePion::OnMoveCompleted, 0.5f, false);

	
	FVector NewLocation = CalculateArcPosition(MoveProgress);
	SetActorLocation(NewLocation);

	
	MoveProgress += 0.05f;
}

void ADamePion::OnMoveCompleted()
{
	// Mettre à jour la position finale du mouvement
	SetActorLocation(TargetPosition);
	UE_LOG(LogTemp, Display, TEXT("OnMoveCompleted"));
	if (OpponentsToDestroy.Num() > 0)
	{
		// Destruction du pion adverse
		OpponentsToDestroy[0]->Die();
		OpponentsToDestroy.RemoveAt(0);
	}

	if (MoveSequence.Num() > 0)
	{
		// Reconfigurer le mouvement pour la prochaine case dans la séquence
		StartPosition = TargetPosition;
		TargetPosition = MoveSequence[0];
		MoveSequence.RemoveAt(0);
		MoveToNextPosition(); // Appeler la prochaine séquence
	}
	else
	{
		BecameAQueen();
		// Séquence terminée
		GetWorldTimerManager().ClearTimer(MovementTimer);
		ChangeMaterial(false);
		Board->DameGM->ChangePlayer();
	}
}

FVector ADamePion::CalculateArcPosition(float LerpValue) const
{
	// Calculer une position interpolée entre StartPosition et TargetPosition
	FVector MidPoint = (StartPosition + TargetPosition) * 0.5f;
	MidPoint.Z += 100; // Élever le point intermédiaire pour former un arc

	// Interpolation quadratique pour simuler un arc en forme de cloche
	FVector FirstLerp = FMath::Lerp(StartPosition, MidPoint, LerpValue);
	FVector SecondLerp = FMath::Lerp(MidPoint, TargetPosition, LerpValue);
	return FMath::Lerp(FirstLerp, SecondLerp, LerpValue);
}

void ADamePion::ChangeMaterial(bool isHightlightActive = false)
{
	if(isHightlightActive)
	{
		StaticMesh->SetMaterial(0,HighlightMaterial);
	} else if(PlayerId == 1)
	{
		StaticMesh->SetMaterial(0,FirstPlayerMaterial);
	} else
	{
		StaticMesh->SetMaterial(0,SecondPlayerMaterial);
	}
}

// Called every frame
void ADamePion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

