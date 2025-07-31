// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/MouseResponsive.h"
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

private:
	void TraceUnderMouse();
	void ClickUnderMouse();

public:
	UPROPERTY(EditAnywhere)
	float MouseTraceLength = 2000.f;

	UPROPERTY(EditAnywhere)
	class UInputAction* InteractOnMapAction;

private:
	UPROPERTY()
	TScriptInterface<IMouseResponsive> CurrentMouseResponsive;
};
