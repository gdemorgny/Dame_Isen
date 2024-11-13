// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global/ISelectable.h"
#include "DameBoard.h"
#include "DamePion.generated.h"

UCLASS()
class DAME_ISEN_API ADamePion : public AActor, public IISelectable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamePion();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 CellNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CaptureMaxAllowed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsQueen;

	//TODO : Acces au board
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ADameBoard* Board;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* FirstPlayerMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* SecondPlayerMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* HighlightMaterial;
	
	//TODO : fonction Mourrir
	UFUNCTION(BlueprintCallable)
	void Die();
	//TODO : fonction Grandir
	UFUNCTION(BlueprintCallable)
	void BecameAQueen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Select() override;
	virtual void Unselect() override;
	void StartMoveSequence(const TArray<FVector>& InMoveSequence, TArray<ADamePion*> InOpponentToDestroy);
	UFUNCTION(BlueprintCallable)
	void ChangeMaterial(bool isHightlightActive);
private :
	void MoveToNextPosition();        // Fonction pour gérer le déplacement vers la prochaine case
	void OnMoveCompleted();           // Fonction appelée à la fin de chaque mouvement
	FVector CalculateArcPosition(float LerpValue) const; // Fonction pour calculer la position en arc

	TArray<FVector> MoveSequence;     // Liste des positions de mouvement
	TArray<ADamePion*> OpponentsToDestroy;     // Pointeur vers le pion adverse à détruire

	FVector StartPosition;            // Position de départ pour le mouvement en cours
	FVector TargetPosition;           // Prochaine position cible
	FTimerHandle MovementTimer;       // Timer pour gérer le délai entre chaque mouvement
	float MoveProgress;               // Progression du mouvement [0,1]
	
};
