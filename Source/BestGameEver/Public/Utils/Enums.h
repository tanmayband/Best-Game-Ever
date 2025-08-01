#pragma once
#include "CoreMinimal.h"
#include "Enums.generated.h"

UENUM()
enum class EControllerAction : uint8
{
	Nothing,
	BuildingPathway
};

UENUM()
enum class EMapNodeState : uint8
{
	Disabled,
	Ready,
	Hovered,
	Activated
};

UENUM(BlueprintType)
enum class EMapNodeType : uint8
{
	Start,
	Pitspot,
	End
};

UENUM()
enum class EMapPathwayState : uint8
{
	Building,
	Ready
};

UENUM()
enum class EMapNavigatorState : uint8
{
	Ready,
	Moving,
	Paused,
	Stopped
};