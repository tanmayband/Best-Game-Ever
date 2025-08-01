// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/Enums.h"
#include "MapNavigator.generated.h"

class AMapNode;

UCLASS()
class BESTGAMEEVER_API AMapNavigator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapNavigator();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	class USceneComponent* SceneRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* NavigatorMesh;

	UPROPERTY(EditAnywhere)
	float Speed = 10.f; // units/sec

	UPROPERTY(EditAnywhere)
	AMapNode* StartNode;

private:
	EMapNavigatorState CurrentState;

	UPROPERTY()
	AMapNode* PreviousNode;
	UPROPERTY()
	AMapNode* NextNode;
	
public:
	UFUNCTION(BlueprintCallable)
	void StartMoving();
};
