// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DameGameMode.generated.h"

/**
 * 
 */

class ADameGameMode;
USTRUCT(BlueprintType)
struct FPlayerPawns {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class ADamePion*> Pawns;
	
};

USTRUCT(BlueprintType)
struct FLinksPlayerCell {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PawnId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CellId;
	
};

USTRUCT(BlueprintType)
struct FDameGameState {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerPawns> PlayerPawns;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class ADameCase*> Cells;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLinksPlayerCell> Links;
	UPROPERTY()
	int32 ActualPlayer = 1;
};
UCLASS()
class DAME_ISEN_API ADameGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	
public:
	
	//TODO : fonction d'initialisation du plateau 
	UFUNCTION(BlueprintCallable)
	void InitializeBoard();
	//TODO : fonction Initialisation des joueurs
	UFUNCTION(BlueprintCallable)
	void InitializePlayers();
	//TODO : liste des joueurs / joueur actuel
	
	UPROPERTY(BlueprintReadOnly)
	int32 CaptureMandatoryNumber = 0;
	//TODO : Fonction Changement / alternance de joueur
	UFUNCTION(BlueprintCallable)
	void ChangePlayer();
	UFUNCTION(BlueprintCallable)
	void CheckMandatoryCapture();
	UFUNCTION()
	ADamePion* SpawnPion(int32 CellNumber, bool PawnColorType);
	 
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ADameBoard* Board;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AActor> PionActor;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AActor> CaseActor;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	uint8 BoardSize = 8;

private:
	UPROPERTY()
	FDameGameState DameGameState;
	
public:
	UFUNCTION()
	int32 GetActualPlayer() const;
	UFUNCTION()
	bool IsPawnControlledByActualPlayer(int32 PawnId);
	UFUNCTION()
	int32 GetPlayerIdByPawnId(int32 PawnId);
	UFUNCTION()
	int32 GetCellIdByPawnId(int32 PawnId);
	UFUNCTION()
	ADameCase* GetCellByPawnId(int32 PawnId);
	UFUNCTION()
	ADameCase* GetCellByCellId(int32 CellId);
	UFUNCTION()
	bool IsCellEmpty(int32 CellId);
	UFUNCTION()
	int32 GetPawnIdByCellNumber(int32 CellId);
	UFUNCTION()
	ADamePion* GetPawnByCellNumber(int32 CellId);
	UFUNCTION()
	int32 GetPawnNumberByPlayerId(int32 PlayerId);
	UFUNCTION()
	TArray<ADamePion*> GetPawnsByPlayerId(int32 PlayerId);
	UFUNCTION()
	void RemovePawnAtIndexByPlayerId(int32 PlayerId,int32 index);
	UFUNCTION()
	void RemovePawnByPawnId(int32 PawnId);
	UFUNCTION()
	void RemoveLinkByPawnId(int32 PawnId);
	UFUNCTION()
	void RemoveLinkByCellId(int32 CellId);
	UFUNCTION()
	void SetNewLink(int32 PawnId,int32 CellId);
};
