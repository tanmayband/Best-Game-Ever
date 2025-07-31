// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/MouseResponsive.h"
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
	// bool 
	
protected:
	virtual void OnMouseHover_Implementation() override;
	virtual void OnMouseHoverStop_Implementation() override;
	virtual void OnMouseClick_Implementation() override;

};
