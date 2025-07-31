// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapSpot.h"

// Sets default values
AMapSpot::AMapSpot()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneRoot);

	UpdateState(EMapSpotState::Ready);
}

// Called when the game starts or when spawned
void AMapSpot::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AMapSpot::OnMouseHover_Implementation()
{
	if(CurrentState == EMapSpotState::Ready)
	{
		UE_LOG(LogTemp, Log, TEXT("Hovering over %s"), *GetDebugName(this));
		UpdateState(EMapSpotState::Hovered);
		return true;
	}
	return false;
}

bool AMapSpot::OnMouseHoverStop_Implementation()
{
	if(CurrentState == EMapSpotState::Hovered)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hovering away from %s"), *GetDebugName(this));
		UpdateState(EMapSpotState::Ready);
		return true;
	}
	return false;
}

bool AMapSpot::OnMouseClick_Implementation()
{
	if(!IsDisabled())
	{
		UE_LOG(LogTemp, Log, TEXT("Clicked on %s"), *GetDebugName(this));
		if(CurrentState != EMapSpotState::Activated)
			ToggleActivate(true, true);
		else
			ToggleActivate(false, true);
		
		return true;
	}
	return false;
}

void AMapSpot::UpdateState(const EMapSpotState newState)
{
	CurrentState = newState;
}

bool AMapSpot::IsDisabled()
{
	return CurrentState == EMapSpotState::Disabled;
}

void AMapSpot::ToggleActivate_Implementation(const bool activate, const bool fromClick)
{
	EMapSpotState deactivatedState = fromClick ? EMapSpotState::Hovered : EMapSpotState::Ready;
	UpdateState(activate ? EMapSpotState::Activated : deactivatedState);
}

// Called every frame
void AMapSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

