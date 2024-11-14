// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global/DameGameMode.h"
#include "DameBoard.generated.h"

class ADameBoard;

UENUM(BlueprintType)
enum class ECellState : uint8
{
	Empty UMETA(DisplayName = "Empty"),
	WhitePawn UMETA(DisplayName = "White Pawn"),
	BlackPawn UMETA(DisplayName = "Black Pawn"),
	WhiteQueen UMETA(DisplayName = "White Queen"),
	BlackQueen UMETA(DisplayName = "Black Queen")
};

// Structure pour représenter un état de jeu simplifié


USTRUCT(BlueprintType)
struct FColumnCell {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> Column;
};

USTRUCT(BlueprintType)
struct FMovementSequence {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> Cells;
};

UCLASS()
class DAME_ISEN_API ADameBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADameBoard();
	
	//Game mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ADameGameMode* DameGM;
	
	//Variable suivi de pion sélectionné
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ADamePion* PawnSelected;

	//Liste des mouvements possibles
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMovementSequence> CellValidMoves;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
		
	UFUNCTION()
	bool IsOtherPlayerInCell(int32 cellNumber);
	UFUNCTION()
	bool IsInNextLine(int32 cellNumber,int32 lineNUmber);

	//Fonction Initialisation du plateau
	UFUNCTION(BlueprintCallable)
	void IntializeBoard();
	//Fonction Estimation deplacement possible
	UFUNCTION()
	TArray<FMovementSequence> CheckMoves(int32 cellStart,int32 playerId,bool alreadyEat);
	UFUNCTION()
	TArray<FMovementSequence> CheckPawnMove(int32 cellStart,int32 playerId,bool alreadyEat);
	UFUNCTION()
	TArray<FMovementSequence> CheckDameMove(int32 cellStart,int32 playerId,bool alreadyEat,FMovementSequence prevMoves);
	//fonction dire � une case qu'elle est s�lectionnable -> Interface
	UFUNCTION()
	void CellHighlight(int32 cellNumber,bool IsHighLighting = false);
	

	//Fonction d'informaiton, nouveau pion sélectionné.
	UFUNCTION()
	void NewPawnSelect(class ADamePion* newPawn);

	//Fonction d'informaiton, nouvelle case sélectionnée.
	UFUNCTION()
	void NewCellSelect(class ADameCase* newCell);

	

};
