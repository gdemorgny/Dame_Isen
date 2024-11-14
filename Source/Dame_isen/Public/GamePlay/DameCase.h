// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global/ISelectable.h"
#include "DameBoard.h"
#include "DameCase.generated.h"

UCLASS()
class DAME_ISEN_API ADameCase : public AActor, public IISelectable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADameCase();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CellId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CellColorType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* WhiteBaseMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* BlackBaseMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* HighlightMaterial;
	//TODO : Acces au board
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ADameBoard* Board;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ChangeMaterial(bool isHightlightActive);
	
	virtual void Select() override;
	virtual void Unselect() override;
};
