// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/MouseResponsive.h"
#include "Utils/Enums.h"
#include "BestGameController.generated.h"

/**
 * 
 */
UCLASS()
class BESTGAMEEVER_API ABestGameController : public APlayerController
{
	GENERATED_BODY()

public:
	ABestGameController();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

public:
	UPROPERTY(EditAnywhere)
	float MouseTraceLength = 3000.f;

	UPROPERTY(EditAnywhere)
	class UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere)
	class UInputAction* InteractOnMapAction;

private:
	UPROPERTY()
	TScriptInterface<IMouseResponsive> CurrentMouseResponsive;
	FHitResult CurrentHitResult;
	UPROPERTY()
	class AMapNode* CurrentMapNodeBuilding;

	EControllerAction CurrentAction;

private:
	void TraceUnderMouse();
	void ClickUnderMouse();
};
