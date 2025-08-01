// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/MouseResponsive.h"
#include "Utils/Enums.h"
// #include "Actors/Map/MapPathway.h"
#include "MapNode.generated.h"

UCLASS()
class BESTGAMEEVER_API AMapNode : public AActor, public IMouseResponsive
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapNode();
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

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMapPathway> MapPathwayClass;

	UPROPERTY(EditAnywhere)
	EMapNodeType NodeType = EMapNodeType::Pitspot;

private:
	EMapNodeState CurrentState;
	UPROPERTY()
	class AMapPathway* CurrentBuildingPathway;
	UPROPERTY()
	TArray<AMapPathway*> ConnectedPathways;
	// UPROPERTY()
	// TArray<AMapNode*> ConnectedNodes;
	
public:
	void StartBuildingPathway();
	void UpdatePathwayEndPoint(const FVector& location);
	void UpdatePathwayEndNode(AMapNode* newNode);
	void StopBuildingPathway();
	void UnlinkPathway(AMapPathway* pathway);
	FORCEINLINE TArray<AMapPathway*> GetConnectedPathways();
	void AddPathway(AMapPathway* pathway);
	
protected:
	virtual bool OnMouseHover_Implementation() override;
	virtual bool OnMouseHoverStop_Implementation() override;
	virtual bool OnMouseClick_Implementation() override;
	virtual bool OnMouseClickStop_Implementation() override;

	void UpdateState(const EMapNodeState newState);
	bool IsDisabled();
	UFUNCTION(BlueprintNativeEvent)
	void ToggleActivate(const bool activate);

private:
	void OrientPathway(const FVector& location);
	bool ArePathwaysSame(AMapPathway* pathway1, AMapPathway* pathway2);
	void DeleteCurrentPathway();
	void UpdateActivateStatus();

};
