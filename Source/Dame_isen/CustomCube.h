// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomCube.generated.h"

UCLASS()
class DAME_ISEN_API ACustomCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomCube();
	UFUNCTION(BlueprintCallable)
	void ChangeColor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CubeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* CustomMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor FinalColor;
	

	
};
