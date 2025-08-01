// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapNavigator.h"
#include "Actors/Map/MapNode.h"
#include "Actors/Map/MapPathway.h"

// Sets default values
AMapNavigator::AMapNavigator()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	NavigatorMesh = CreateDefaultSubobject<UStaticMeshComponent>("NavigatorMesh");
	NavigatorMesh->SetupAttachment(SceneRoot);
	NavigatorMesh->SetRelativeLocation(FVector(0, 0, 50));
	NavigatorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AMapNavigator::BeginPlay()
{
	Super::BeginPlay();

	if(StartNode)
	{
		PreviousNode = StartNode;
		SetActorLocation(PreviousNode->GetActorLocation());
		CurrentState = EMapNavigatorState::Ready;
	}
}

void AMapNavigator::StartMoving()
{
	if(PreviousNode)
	{
		TArray<AMapPathway*> previousPathways = PreviousNode->GetConnectedPathways();
		if(!previousPathways.IsEmpty())
		{
			AMapPathway* nextPathway;
			if(previousPathways.IsValidIndex(1))
				nextPathway = previousPathways[1];
			else
				nextPathway = previousPathways[0];
			
			FPathwayNodes pathwayNodes;
			nextPathway->GetConnectedNodes(pathwayNodes);
			if(pathwayNodes.EndNode != PreviousNode)
			{
				NextNode = pathwayNodes.EndNode;
				CurrentState = EMapNavigatorState::Moving;
			}
		}
	}
}

// Called every frame
void AMapNavigator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
		case EMapNavigatorState::Moving:
		{
			const FVector targetLocation = NextNode->GetActorLocation();
			const FVector newLocation = FMath::VInterpConstantTo(GetActorLocation(), NextNode->GetActorLocation(), DeltaTime, Speed);
			SetActorLocation(newLocation);

			if (FVector::DistSquared(newLocation, targetLocation) < 1.f)
			{
				CurrentState = EMapNavigatorState::Paused;
			}
			break;
		}
		case EMapNavigatorState::Paused:
		{
			PreviousNode = NextNode;
			// StartMoving();
			break;
		}
	}
}

