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
	UPROPERTY()
	int32 ActualPlayer = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerPawns> PlayerPawns;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<int32> Players;
	UPROPERTY(BlueprintReadOnly)
	int32 CaptureMandatoryNumber = 0;
	//TODO : Fonction Changement / alternance de joueur
	UFUNCTION(BlueprintCallable)
	void ChangePlayer();
	UFUNCTION(BlueprintCallable)
	void CheckMandatoryCapture();
	UFUNCTION()
	void SpawnPion(int32 CellNumber, int32 PlayerId);
	 
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ADameBoard* Board;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AActor> Pion;
};
