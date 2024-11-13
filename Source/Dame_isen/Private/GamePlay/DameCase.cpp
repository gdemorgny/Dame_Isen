// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/DameCase.h"

// Sets default values
ADameCase::ADameCase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADameCase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADameCase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADameCase::ChangeMaterial(bool isHightlightActive)
{
	
	isHightlightActive?StaticMesh->SetMaterial(0,HighlightMaterial):StaticMesh->SetMaterial(0,BaseMaterial);
}

void ADameCase::SetBaseMat()
{
	BaseMaterial = StaticMesh->GetMaterial(0);
}

void ADameCase::Select()
{
	Board->NewCellSelect(this);
}

void ADameCase::Unselect()
{
	
}

