// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapNode.h"
#include "Actors/Map/MapPathway.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMapNode::AMapNode()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(SceneRoot);

	UpdateState(EMapNodeState::Ready);
	ConnectedPathways.Empty();
}

// Called when the game starts or when spawned
void AMapNode::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMapNode::StartBuildingPathway()
{
	// UE_LOG(LogTemp, Log, TEXT("StartBuildingPathway"));
	// spawn a pathway
	if(CurrentBuildingPathway)
		DeleteCurrentPathway();

	if(MapPathwayClass)
	{
		CurrentBuildingPathway = GetWorld()->SpawnActor<AMapPathway>(
		  MapPathwayClass,
		  GetActorTransform()
	  );
		CurrentBuildingPathway->SetStartNode(this);

		UpdateActivateStatus();
	}
}

void AMapNode::UpdatePathwayEndPoint(const FVector& location)
{
	// scale and orient the path
	if(CurrentBuildingPathway)
	{
		// UE_LOG(LogTemp, Warning, TEXT("UpdatePathwayEndPoint %s"), *location.ToString());
		OrientPathway(location);
	}
}

void AMapNode::UpdatePathwayEndNode(AMapNode* newNode)
{
	// if node different from currently added to pathway,
	// add to pathway
	if(CurrentBuildingPathway && !CurrentBuildingPathway->IsConnectedToNode(newNode))
	{
		CurrentBuildingPathway->SetEndNode(newNode);
	}
}

void AMapNode::StopBuildingPathway()
{
	// check if pathway's end node present
		// if yes, check if that node is already connected to me
		// if no, add data to everything
	// if no, destroy pathway
	if(CurrentBuildingPathway)
	{
		FPathwayNodes pathwayNodes;
		CurrentBuildingPathway->GetConnectedNodes(pathwayNodes);
		if(!CurrentBuildingPathway->IsPathwayObstructed() && pathwayNodes.EndNode)
		{
			bool isUnconnectedNode = true;
			for(AMapPathway* nodePathway : ConnectedPathways)
			{
				if(nodePathway && nodePathway->IsConnectedToNode(pathwayNodes.EndNode))
				{
					isUnconnectedNode = false;
					break;
				}
			}
			if(isUnconnectedNode)
			{
				AddPathway(CurrentBuildingPathway);
				pathwayNodes.EndNode->AddPathway(CurrentBuildingPathway);
				OrientPathway(pathwayNodes.EndNode->GetActorLocation());
				CurrentBuildingPathway->PathwayReady();
				CurrentBuildingPathway = nullptr;
			}
			else
			{
				DeleteCurrentPathway();
			}
		}
		else
			DeleteCurrentPathway();
	}
	
}

void AMapNode::UnlinkPathway(AMapPathway* pathway)
{
	if(ConnectedPathways.Contains(pathway))
		ConnectedPathways.Remove(pathway);
	UpdateActivateStatus();
}

TArray<AMapPathway*> AMapNode::GetConnectedPathways()
{
	return ConnectedPathways;
}

void AMapNode::AddPathway(AMapPathway* pathway)
{
	ConnectedPathways.AddUnique(pathway);
	UpdateActivateStatus();
}

bool AMapNode::OnMouseHover_Implementation()
{
	if(CurrentState == EMapNodeState::Ready)
	{
		// UE_LOG(LogTemp, Log, TEXT("Hovering over %s"), *GetDebugName(this));
		UpdateState(EMapNodeState::Hovered);
		return true;
	}
	return false;
}

bool AMapNode::OnMouseHoverStop_Implementation()
{
	if(CurrentState == EMapNodeState::Hovered)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Hovering away from %s"), *GetDebugName(this));
		UpdateState(EMapNodeState::Ready);
		return true;
	}
	return false;
}

bool AMapNode::OnMouseClick_Implementation()
{
	if(!IsDisabled())
	{
		// UE_LOG(LogTemp, Log, TEXT("Clicked on %s"), *GetDebugName(this));
		
		return true;
	}
	return false;
}

bool AMapNode::OnMouseClickStop_Implementation()
{
	return IMouseResponsive::OnMouseClickStop_Implementation();
}

void AMapNode::UpdateState(const EMapNodeState newState)
{
	CurrentState = newState;
}

bool AMapNode::IsDisabled()
{
	return CurrentState == EMapNodeState::Disabled;
}

void AMapNode::OrientPathway(const FVector& location)
{
	const FVector myLocation = GetActorLocation();
	const float sqDistanceToEndPoint = FVector::DistSquaredXY(myLocation, location);
	const float scaleFactor =  FMath::Sqrt(sqDistanceToEndPoint / (10000.f));
	CurrentBuildingPathway->SetActorScale3D(FVector(scaleFactor, 1.f, 1.f));

	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(myLocation, FVector(location.X, location.Y, myLocation.Z));
	CurrentBuildingPathway->SetActorRotation(newRotation);
}

bool AMapNode::ArePathwaysSame(AMapPathway* pathway1, AMapPathway* pathway2)
{
	bool arePathwaysSame = false;
	FPathwayNodes pathway1Nodes, pathway2Nodes;
	pathway1->GetConnectedNodes(pathway1Nodes);
	pathway2->GetConnectedNodes(pathway2Nodes);

	arePathwaysSame =
		(
			(pathway1Nodes.StartNode == pathway2Nodes.StartNode) &&
			(pathway1Nodes.EndNode == pathway2Nodes.EndNode)
		) ||
		(
			(pathway1Nodes.StartNode == pathway2Nodes.EndNode) &&
			(pathway1Nodes.EndNode == pathway2Nodes.StartNode)
		);
	return arePathwaysSame;
}

void AMapNode::DeleteCurrentPathway()
{
	GetWorld()->DestroyActor(CurrentBuildingPathway);
	CurrentBuildingPathway = nullptr;
	UpdateActivateStatus();
}

void AMapNode::UpdateActivateStatus()
{
	if(ConnectedPathways.Num() > 0 || CurrentBuildingPathway)
	{
		ToggleActivate(true);
	}
	else
	{
		ToggleActivate(false);
	}
}

void AMapNode::ToggleActivate_Implementation(const bool activate)
{
	// EMapNodeState deactivatedState = fromClick ? EMapNodeState::Hovered : EMapNodeState::Ready;
	UpdateState(activate ? EMapNodeState::Activated : EMapNodeState::Ready);
}

// Called every frame
void AMapNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

