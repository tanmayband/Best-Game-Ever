// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/Structs.h"
#include "MapPathway.generated.h"

UCLASS()
class BESTGAMEEVER_API AMapPathway : public AActor, public IMouseResponsive
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapPathway();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	class USceneComponent* SceneRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* PathwayMesh;
private:
	uint16 NumPathsAwayFromEnd = 0;
	UPROPERTY()
	FPathwayNodes ConnectedNodes;
	bool IsObstructed;

public:
	void GetConnectedNodes(FPathwayNodes& connectedNodes);
	bool IsConnectedToNode(AMapNode* nodeToCheck);
	void SetStartNode(AMapNode* newNode);
	void SetEndNode(AMapNode* newNode);
	UFUNCTION(BlueprintNativeEvent)
	void PathwayReady();
	FORCEINLINE bool IsPathwayObstructed();

protected:
	virtual bool OnMouseClick_Implementation() override;
	UFUNCTION(BlueprintNativeEvent)
	void UpdatePathwayObstructed(bool obstructed);

private:
	void ToggleTraceCollision(const bool active);
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);
	void CheckOverlappingActors();
};
