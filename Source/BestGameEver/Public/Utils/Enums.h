#pragma once
#include "CoreMinimal.h"
#include "Enums.generated.h"

UENUM()
enum EMapNodeState : uint8
{
	Disabled,
	Ready,
	Hovered,
	Activated
};

UENUM()
enum EControllerAction : uint8
{
	Nothing,
	BuildingPathway
};