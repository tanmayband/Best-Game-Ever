// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Players/BestGameController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
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
			FHitResult traceHitResult;
			world->LineTraceSingleByChannel(
				traceHitResult,
				mouseWorldPosition,
				traceEnd,
				ECC_Visibility
			);

			// DrawDebugLine(GetWorld(), mouseWorldPosition, traceEnd, FColor::Black, true);

			if(traceHitResult.bBlockingHit)
			{
				AActor* newHitActor = traceHitResult.GetActor();
				AActor* currentActor = CurrentMouseResponsive ? Cast<AActor>(CurrentMouseResponsive.GetObject()) : nullptr;
				
				if(newHitActor != currentActor)
				{
					if(CurrentMouseResponsive)
					{
						IMouseResponsive::Execute_OnMouseHoverStop(currentActor);
						CurrentMouseResponsive = nullptr;
					}
					if(newHitActor->Implements<UMouseResponsive>())
					{
						CurrentMouseResponsive = newHitActor;
						IMouseResponsive::Execute_OnMouseHover(newHitActor);
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
		IMouseResponsive::Execute_OnMouseClick(CurrentMouseResponsive.GetObject());
	}
}
