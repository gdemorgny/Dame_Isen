// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Global/ISelectable.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DamePlayerController.generated.h"
/**
 * 
 */
UCLASS()
class DAME_ISEN_API ADamePlayerController : public APlayerController
{
	GENERATED_BODY()

	//TODO : Setup inputs
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	UFUNCTION()
	void TrySelect();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* GameplayMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Select;

	UPROPERTY()
	TScriptInterface<IISelectable> Selected;
	//TODO : fonction de selection (globale pion et case) - > Interface
	//TODO : fonction de survol (globale pion et case) -> Component
};