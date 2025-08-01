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
	PathwayMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	ToggleTraceCollision(false);
}

// Called when the game starts or when spawned
void AMapPathway::BeginPlay()
{
	Super::BeginPlay();

	CurrentState = EMapPathwayState::Building;
	
	PathwayMesh->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnOverlap);
	PathwayMesh->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnOverlapEnd);
	
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

bool AMapPathway::IsPathwayIllegal()
{
	return IsObstructed;
}

void AMapPathway::PathwayReady_Implementation()
{
	CurrentState = EMapPathwayState::Ready;
	ToggleTraceCollision(true);
}

void AMapPathway::UpdatePathwayObstructed_Implementation(bool obstructed)
{
	IsObstructed = obstructed;
}

bool AMapPathway::OnMouseClick_Implementation()
{
	Destroy();
	return true;
}

void AMapPathway::ToggleTraceCollision(const bool active)
{
	if(active)
	{
		PathwayMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	}
	else
	{
		PathwayMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	}
}

void AMapPathway::OnOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if(CurrentState != EMapPathwayState::Ready)
		CheckOverlappingActors();
}

void AMapPathway::OnOverlapEnd(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if(CurrentState != EMapPathwayState::Ready)
		CheckOverlappingActors();
}

void AMapPathway::CheckOverlappingActors()
{
	TArray<AActor*> currentOverlappingActors;
	PathwayMesh->GetOverlappingActors(currentOverlappingActors);
	for(const AActor* overlappingActor : currentOverlappingActors)
	{
		if(!(overlappingActor->IsA<AMapNode>() || overlappingActor->IsA<AMapPathway>()))
		{
			UpdatePathwayObstructed(true);
			break;
		}
		
		UpdatePathwayObstructed(false);
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

	if(ConnectedNodes.StartNode)
		ConnectedNodes.StartNode->UnlinkPathway(this);
	if(ConnectedNodes.EndNode)
		ConnectedNodes.EndNode->UnlinkPathway(this);
}

