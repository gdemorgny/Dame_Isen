// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCube.h"

// Sets default values
ACustomCube::ACustomCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ACustomCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACustomCube::ChangeColor()
{
	CustomMat = UMaterialInstanceDynamic::Create(Material, CubeMesh);
	CubeMesh->SetMaterial(0, CustomMat);
	CustomMat->SetVectorParameterValue("BaseColor", FinalColor);

}

