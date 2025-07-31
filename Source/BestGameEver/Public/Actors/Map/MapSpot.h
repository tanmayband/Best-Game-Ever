// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/MouseResponsive.h"
#include "Utils/Enums.h"
#include "MapSpot.generated.h"

UCLASS()
class BESTGAMEEVER_API AMapSpot : public AActor, public IMouseResponsive
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapSpot();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	class USceneComponent* SceneRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

private:
	EMapSpotState CurrentState;
	
protected:
	virtual bool OnMouseHover_Implementation() override;
	virtual bool OnMouseHoverStop_Implementation() override;
	virtual bool OnMouseClick_Implementation() override;

	void UpdateState(const EMapSpotState newState);
	bool IsDisabled();
	UFUNCTION(BlueprintNativeEvent)
	void ToggleActivate(const bool activate, const bool fromClick = false);

};
