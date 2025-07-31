#pragma once
#include "CoreMinimal.h"

UENUM()
enum EMapSpotState : uint8
{
	Disabled,
	Ready,
	Hovered,
	Activated
};