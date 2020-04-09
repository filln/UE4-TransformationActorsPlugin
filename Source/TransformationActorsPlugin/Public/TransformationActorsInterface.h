// Copyright 2020 Anatoli Kucharau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TransformationActorsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Category = "TransformationActorsInterface", Blueprintable)
class UTransformationActorsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TRANSFORMATIONACTORSPLUGIN_API ITransformationActorsInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/*To tell the actor that he was chosen to transform.*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "TransformationActorsInterface")
		void HighlightOn();

	/*Tell the actor that he's been removed from the selected ones.*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "TransformationActorsInterface")
		void HighlightOff();

	/*Tell the actor that he's being transformed. Called before the transformation timers start.*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "TransformationActorsInterface")
		void StartTransformation();

	/*Tell the actor that he's finished transforming. Called before the transformation timers stop.*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "TransformationActorsInterface")
		void StopTransformation();
};
