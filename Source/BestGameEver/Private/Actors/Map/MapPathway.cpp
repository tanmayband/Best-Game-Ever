// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapPathway.h"
#include "Actors/Map/MapNode.h"

// Sets default values
AMapPathway::AMapPathway()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	PathwayMesh = CreateDefaultSubobject<UStaticMeshComponent>("PathwayMesh");
	PathwayMesh->SetupAttachment(SceneRoot);
	PathwayMesh->SetRelativeLocation(FVector(50.f, 0, 0));
	ToggleCollision(false);
}

// Called when the game starts or when spawned
void AMapPathway::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMapPathway::GetConnectedNodes(FPathwayNodes& connectedNodes)
{
	connectedNodes = ConnectedNodes;
}

bool AMapPathway::IsConnectedToNode(AMapNode* nodeToCheck)
{
	return ConnectedNodes.StartNode == nodeToCheck || ConnectedNodes.EndNode == nodeToCheck;
}

void AMapPathway::SetStartNode(AMapNode* newNode)
{
	ConnectedNodes.StartNode = newNode;
}

void AMapPathway::SetEndNode(AMapNode* newNode)
{
	ConnectedNodes.EndNode = newNode;
}

bool AMapPathway::OnMouseClick_Implementation()
{
	Destroy();
	return true;
}


void AMapPathway::ToggleCollision(const bool active)
{
	if(active)
	{
		PathwayMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		PathwayMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called every frame
void AMapPathway::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapPathway::Destroyed()
{
	Super::Destroyed();

	ConnectedNodes.StartNode->UnlinkPathway(this);
	ConnectedNodes.EndNode->UnlinkPathway(this);
}

