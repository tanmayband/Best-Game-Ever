#pragma once
#include "CoreMinimal.h"
#include "Actors/Map/MapNode.h"
#include "Structs.generated.h"

USTRUCT()
struct FPathwayNodes
{
	GENERATED_BODY()

	UPROPERTY()
	AMapNode* StartNode;
	UPROPERTY()
	AMapNode* EndNode;
};