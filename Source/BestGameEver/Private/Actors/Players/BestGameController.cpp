// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Players/BestGameController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/Map/MapNode.h"
#include "Interfaces/MouseResponsive.h"

ABestGameController::ABestGameController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	// AutoReceiveInput = EAutoReceiveInput::Player0;
}

void ABestGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceUnderMouse();
}

void ABestGameController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		inputSubsystem->AddMappingContext(InputMappingContext, 0);
	}

	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		UE_LOG(LogTemp, Log, TEXT("Setup enhanced InputComponent"));
		enhancedInputComponent->BindAction(InteractOnMapAction, ETriggerEvent::Started, this, &ThisClass::ClickUnderMouse);
	}
}

void ABestGameController::TraceUnderMouse()
{
	FVector mouseWorldPosition, mouseWorldDirection;
	if(const UWorld* world = GetWorld(); DeprojectMousePositionToWorld(mouseWorldPosition, mouseWorldDirection))
	{
		{
			const FVector traceEnd = mouseWorldPosition + (mouseWorldDirection * MouseTraceLength);
			world->LineTraceSingleByChannel(
				CurrentHitResult,
				mouseWorldPosition,
				traceEnd,
				ECC_GameTraceChannel1
			);

			// DrawDebugLine(GetWorld(), mouseWorldPosition, traceEnd, FColor::Black, true);
			AActor* newHitActor = CurrentHitResult.GetActor();
			AActor* currentActor = Cast<AActor>(CurrentMouseResponsive.GetObject());
			// For visual effects
			if(!CurrentHitResult.bBlockingHit)
			{
				// hovered over nothing?
				if(currentActor)
				{
					IMouseResponsive::Execute_OnMouseHoverStop(currentActor);
					CurrentMouseResponsive = nullptr;
				}
			}
			else
			{
				// hover over new thing?
				if(currentActor && (currentActor != newHitActor))
					IMouseResponsive::Execute_OnMouseHoverStop(currentActor);
				
				if(newHitActor->Implements<UMouseResponsive>())
				{
					CurrentMouseResponsive = newHitActor;
					IMouseResponsive::Execute_OnMouseHover(newHitActor);
				}
				else
				{
					CurrentMouseResponsive = nullptr;
				}
			}

			// Process map elements
			//
			// hovered over nothing?
			if(!CurrentHitResult.bBlockingHit)
			{
				if(CurrentAction == EControllerAction::BuildingPathway)
				{
					if(CurrentMapNodeBuilding)
					{
						CurrentMapNodeBuilding->UpdatePathwayEndNode(nullptr);
					}
				}
			}
			else
			{				
				if(newHitActor)
				{
					if(CurrentAction == EControllerAction::BuildingPathway)
					{
						if(CurrentMapNodeBuilding)
						{
							CurrentMapNodeBuilding->UpdatePathwayEndPoint(CurrentHitResult.ImpactPoint);
						}
					}
				}
				// UE_LOG(LogTemp, Log, TEXT("%s"), *GetDebugName(traceHitResult.GetActor()));
			}
		}
	}
}

void ABestGameController::ClickUnderMouse()
{
	UE_LOG(LogTemp, Log, TEXT("ClickUnderMouse"));
	if(CurrentMouseResponsive)
	{
		if(CurrentAction == EControllerAction::BuildingPathway)
		{
			if(CurrentMapNodeBuilding)
			{
				CurrentAction = EControllerAction::Nothing;
				if(AMapNode* newMapNode = Cast<AMapNode>(CurrentMouseResponsive.GetObject()))
				{
					CurrentMapNodeBuilding->UpdatePathwayEndNode(newMapNode);
				}
				CurrentMapNodeBuilding->StopBuildingPathway();
				CurrentMapNodeBuilding = nullptr;
			}
		}
		else
		{
			CurrentMapNodeBuilding = Cast<AMapNode>(CurrentMouseResponsive.GetObject());
			if(CurrentMapNodeBuilding)
			{
				CurrentAction = EControllerAction::BuildingPathway;
				CurrentMapNodeBuilding->StartBuildingPathway();
				CurrentMapNodeBuilding->UpdatePathwayEndPoint(CurrentHitResult.ImpactPoint);
			}			
		}
		IMouseResponsive::Execute_OnMouseClick(CurrentMouseResponsive.GetObject());
	}
}
