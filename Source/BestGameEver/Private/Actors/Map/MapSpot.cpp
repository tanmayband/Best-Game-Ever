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

}

// Called when the game starts or when spawned
void AMapSpot::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMapSpot::OnMouseHover_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Hovering over %s"), *GetDebugName(this));
}

void AMapSpot::OnMouseHoverStop_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Hovering away from %s"), *GetDebugName(this));
}

void AMapSpot::OnMouseClick_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Clicked on %s"), *GetDebugName(this));
}

// Called every frame
void AMapSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

