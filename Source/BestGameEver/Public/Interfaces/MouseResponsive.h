// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MouseResponsive.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMouseResponsive : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BESTGAMEEVER_API IMouseResponsive
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category="MouseResponsive")
	void OnMouseHover();
	UFUNCTION(BlueprintNativeEvent, Category="MouseResponsive")
	void OnMouseHoverStop();
	UFUNCTION(BlueprintNativeEvent, Category="MouseResponsive")
	void OnMouseClick();
};
