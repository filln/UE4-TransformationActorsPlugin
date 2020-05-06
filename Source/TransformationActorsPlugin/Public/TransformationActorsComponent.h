// Copyright 2020 Anatoli Kucharau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "TransformationActorsComponent.generated.h"


class APlayerController;
class APawn;

/*The states of the actor through which you can select an operation on it.*/
UENUM(BlueprintType, Category = "TransformationActorsComponent | ETransformState")
enum class ETransformState : uint8
{
	//Translation.
	ETS_Location	UMETA(DisplayName = "Location"),

	//Rotation Yaw and Pitch.
	ETS_Rotation_YawPitch	UMETA(DisplayName = "Rotation_YawPitch"),

	//Rotation Roll.
	ETS_Rotation_Roll		UMETA(DisplayName = "Rotation_Roll"),

	//Rotation Pitch.
	ETS_Rotation_Pitch		UMETA(DisplayName = "Rotation_Pitch"),

	//Rotation Yaw.
	ETS_Rotation_Yaw		UMETA(DisplayName = "Rotation_Yaw"),

	//Scale.
	ETS_Scale				UMETA(DisplayName = "Scale"),

	//Idle. No operations.
	ETS_Idle				UMETA(DisplayName = "Idle")
};

/*Dispatcher that is called when the transformation mode is activated.*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSwitchOnTransformationMode);
/*Dispatcher that is called when the transformation mode is switched off.*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSwitchOffTransformationMode);
/*Dispatcher called before starting the transformation timers.*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartTransformationActor);
/*Dispatcher called before stopping the transformation timers.*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopTransformationActor);

/*Class of the main plugin component.*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TRANSFORMATIONACTORSPLUGIN_API UTransformationActorsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTransformationActorsComponent();

public:
	//////////////////////////////////////////////////////////////////////////
	/*Public UPROPERTY variables.*/


	/*Dispatcher that is called when the transformation mode is activated.*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "TransformationActorsComponent | Delegates")
		FOnSwitchOnTransformationMode OnSwitchOnTransformationMode;
	/*Dispatcher that is called when the transformation mode is switched off.*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "TransformationActorsComponent | Delegates")
		FOnSwitchOffTransformationMode OnSwitchOffTransformationMode;
	/*Dispatcher called before starting the transformation timers.*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "TransformationActorsComponent | Delegates")
		FOnStartTransformationActor OnStartTransformationActor;
	/*Dispatcher called before stopping the transformation timers.*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "TransformationActorsComponent | Delegates")
		FOnStopTransformationActor OnStopTransformationActor;

	/*The period when the timer for translation actors is triggered.*/
	UPROPERTY(EditAnywhere, Category = "TransformationActorsComponent")
		float LocationTimerDeltaTime;

	/*The period of timer activation of actors rotation.*/
	UPROPERTY(EditAnywhere, Category = "TransformationActorsComponent")
		float RotationTimerDeltaTime;

	/*The period of triggering the actor scaling timer.*/
	UPROPERTY(EditAnywhere, Category = "TransformationActorsComponent")
		float ScaleTimerDeltaTime;

	/*Parameter for VInterpConstantTo, interpolation speed of translation vector.*/
	UPROPERTY(EditAnywhere, Category = "TransformationActorsComponent")
		float LocationSpeed;

	/*If true than actor under cursor or keyboard can't move through other objects . If false than actor can do it*/
	UPROPERTY(EditAnywhere, Category = "TransformationActorsComponent")
		bool bSweep;

	/*The speed of translation in depth (from yourself or to yourself).*/
	UPROPERTY(EditAnywhere, Category = "TransformationActorsComponent")
		float LocationDeepSpeed;

	/*Scaling speed.*/
	UPROPERTY(EditAnywhere, Category = "TransformationActorsComponent")
		float ScaleSpeed;

	/*Speed of rotation.*/
	UPROPERTY(EditAnywhere, Category = "TransformationActorsComponent")
		float RotationSpeed;

	/*Minimum scale with cursor and keyboard.*/
	UPROPERTY(EditAnywhere, Category = "TransformationActorsComponent")
		float MinScale;

	/*Show debug messages.*/
	UPROPERTY(EditAnywhere, Category = "TransformationActorsComponent")
		bool bIsShowDebugMessages;

	/*Speed of translation actor with keyboard.*/
	UPROPERTY(EditAnywhere, Category = "TransformationActorsComponent | Keyboard")
		float LocationSpeedKeyboard;

	/*Speed of rotation actor with keyboard.*/
	UPROPERTY(EditAnywhere, Category = "TransformationActorsComponent | Keyboard")
		float RotationSpeedKeyboard;

	/*Speed of scale actor with keyboard.*/
	UPROPERTY(EditAnywhere, Category = "TransformationActorsComponent | Keyboard")
		float ScaleSpeedKeyboard;
private:
	//////////////////////////////////////////////////////////////////////////
	/*Private variables.*/

	/*Timer for translation actors.*/
	FTimerHandle LocationTimer;

	/*Timer for rotation of actors.*/
	FTimerHandle RotationTimer;

	/*Timer to scale the actors.*/
	FTimerHandle ScaleTimer;

	/*Blocking the actions in the first tick of the LocationTimer.*/
	bool bIsLockFirstIterationLocationTimer;
	/*Lock actions in the first tick of the RotationTimer.*/
	bool bIsLockFirstIterationRotationTimer;
	/*Lock the actions in the first tick of the ScaleTimer.*/
	bool bIsLockFirstIterationScaleTimer;

	/*The states of the actor through which you can select an operation on it.*/
	ETransformState TransformState;

	/*Saves the state of the actors: under the control of the player cursor or not.*/
	bool bIsTransform;

	APlayerController* PlayerController;
	APawn* PlayerPawn;

	/*The component from which the axes of transformation are taken.*/
	USceneComponent* ComponentForTransformationAxis;

	/*An actor controlled by a player.*/
	AActor* TransformActor;
	/*The actor controlled by the player the previous time.*/
	AActor* PreviousTransformActor;

	/*Memorized rotations from the previous CalcDelta...() method call.*/
	/*Вращение вокруг оси Х в градусах.*/
	float RollSave;
	/*Вращение вокруг оси Y в градусах.*/
	float PitchSave;
	/*Вращение вокруг оси Z в градусах.*/
	float YawSave;

	/*The difference between the rotation degrees in the current and previous call of CalcDeltaRoll, CalcDeltaPitch, CalcDeltaYaw methods.*/
	float DeltaRollDegree;
	/*The difference between the rotation degrees in the current and previous call of CalcDeltaRoll, CalcDeltaPitch, CalcDeltaYaw methods.*/
	float DeltaPitchDegree;
	/*The difference between the rotation degrees in the current and previous call of CalcDeltaRoll, CalcDeltaPitch, CalcDeltaYaw methods.*/
	float DeltaYawDegree;

	/*Saving the distance from the mouse cursor to TransformActor, which was found when you first click on TransformActor.*/
	float DistanceToCursorSave;

	/*Saving mouse coordinates when clicking on TransformActor.*/
	float LocationXAtClick;
	float LocationYAtClick;
	/*Save scale when you click on TransformActor.*/
	FVector Scale3DSave;

	/*New scale.*/
	FVector NewScale3D;

	/*The sum of AxisValue values.*/
	float SumInputAxisValue;

	/*Show transformation status: Location left or right.*/
	bool bIsLocationLeftRightKeyboard;
	/*Show transformation status: Location up or down.*/
	bool bIsLocationUpDownKeyboard;
	/*Show transformation status: Location inside or outside.*/
	bool bIsLocationInsideOutsideKeyboard;
	/*Show transformation status: Rotation Roll.*/
	bool bIsRotationRollKeyboard;
	/*Show transformation status: Rotation Pitch.*/
	bool bIsRotationPitchKeyboard;
	/*Show transformation status: Rotation Yaw.*/
	bool bIsRotationYawKeyboard;
	/*Show transformation status: Scale.*/
	bool bIsScaleKeyboard;
	/*Show transformation status: Scale X.*/
	bool bIsScaleXKeyboard;
	/*Show transformation status: Scale Y.*/
	bool bIsScaleYKeyboard;
	/*Show transformation status: Scale Z.*/
	bool bIsScaleZKeyboard;


public:
	//////////////////////////////////////////////////////////////////////////
	/*Public UFUNCTION methods.*/

	/*One of the main methods for starting actor transformations. It is started by pressing the mouse button. It is composed of methods of the "Basic methods" category.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | General methods")
		void StartTransformationActor();

	/*One of the main methods to stop the actor transformations. Starts by releasing the mouse button. It is composed of methods of the "Basic methods" category.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | General methods")
		void StopTransformationActor();

	/*One of the main methods to start the transformation mode.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | General methods")
		void SwitchOnTransformationMode(ETransformState InTransformState);

	/*One of the main methods for exiting the transformation mode.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | General methods")
		void SwitchOffTransformationMode();

	/*Calculate the sum of AxisValue values.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | General methods")
		void CalcSumInputAxisValue(float InputAxisValue);



	/*Location left or right with keyboard. Use Y axe.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | General methods")
		void LocationLeftRightKeyboard(float AxisValue);
	/*Location up or down with keyboard. Use Z axe.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | General methods")
		void LocationUpDownKeyboard(float AxisValue);
	/*Location inside or outside with keyboard. Use X axe.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | General methods")
		void LocationInsideOutsideKeyboard(float AxisValue);
	/*Rotation Roll with keyboard.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | General methods")
		void RotationRollKeyboard(float AxisValue);
	/*Rotation Pitch with keyboard.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | General methods")
		void RotationPitchKeyboard(float AxisValue);
	/*Rotation Yaw with keyboard.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | General methods")
		void RotationYawKeyboard(float AxisValue);
	/*Scale with keyboard along local XYZ axis.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | General methods")
		void ScaleKeyboard(float AxisValue);
	/*Scale with keyboard along local X axe.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | General methods")
		void ScaleXKeyboard(float AxisValue);
	/*Scale with keyboard along local Y axe.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | General methods")
		void ScaleYKeyboard(float AxisValue);
	/*Scale with keyboard along local Z axe.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | General methods")
		void ScaleZKeyboard(float AxisValue);

	/*Location with keyboard. Basic method.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Basic methods")
		void LocationKeyboardBasic(FVector DeltaLocation);

	/*Rotation with keyboard. Basic methods.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Basic methods")
		void RotationKeyboardBasic(float AxisValue, FVector Axe);

	/*Scale with keyboard. Basic methods.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Basic methods")
		void ScaleKeyboardBasic(FVector DeltaScale3D);

	/*Set the input mode to "Game and UI".*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void SetInputModeGameAndUI();

	/*Set the input mode to "Game only".*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void SetInputModeGameOnly();

	/*Set the input mode to "UI only".*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void SetInputModeUIOnly();

	/*Try to find the actor traced by the mouse cursor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		AActor* FindActorUnderCursor();

	/*Run
	StartLocationTimer() or
	StartRotationTimer() or
	StartScaleTimer()
	depending on the TransformState.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void StartTransformTimer(ETransformState CurrentTransformState);

	/*Run LocationTimer with the LocationActor() method.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void StartLocationTimer();

	/*Launch RotationTimer with the RotationActor() methods.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void StartRotationTimer(ETransformState CurrentTransformState);

	/*Run ScaleTimer with ScaleActor() method.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void StartScaleTimer();

	/*Move TransformActor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void LocationActor();

	/*Rotate TransformActor Yaw and Pitch.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void RotationActor();

	/*Scale TransformActor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void ScaleActor();


	/*Stop LocationTimer.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void StopLocationTimer();

	/*Stop RotationTimer.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void StopRotationTimer();

	/*Stop ScaleTimer.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void StopScaleTimer();

	/*Select a new TransformActor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void SelectNewTransformActor(AActor* NewTransformActor);

	/*Call the TransformationActorsInterface method.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void HighlightOn_TransformationActorsInterface(AActor* Actor);

	/*Call the TransformationActorsInterface method.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void HighlightOff_TransformationActorsInterface(AActor* Actor);

	/*Call the TransformationActorsInterface method.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void StartTransformation_TransformationActorsInterface(AActor* Actor);

	/*Call the TransformationActorsInterface method.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void StopTransformation_TransformationActorsInterface(AActor* Actor);

	/*Check the actor for inheritance from the TransformationActorsInterface.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		bool CheckActorOnTransformationActorsInterface(AActor* Actor);

	/*Reset the control.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void ResetTransform();

	/*Take the difference between the cursor coordinates in the current and previous method call (in degrees).*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void CalcDeltaRoll(float Roll);
	/*Take the difference between the cursor coordinates in the current and previous method call (in degrees).*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void CalcDeltaPitch(float Pitch);
	/*Take the difference between the cursor coordinates in the current and previous method call (in degrees).*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		void CalcDeltaYaw(float Yaw);


	/*Specify PlayerController and PlayerPawn.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		bool SpecifyControllerAndPawn();

	/*Check PlayerController and PlayerPawn.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Basic methods")
		bool CheckControllerAndPawn();




protected:
	//////////////////////////////////////////////////////////////////////////
	/*Protected methods.*/

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//////////////////////////////////////////////////////////////////////////
		/* BlueprintCallable getters and setters.*/
public:

	/*Set the TransformActor that the player will control.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetTransformActor(AActor* InTransformActor) { TransformActor = InTransformActor; }
	/*Get TransformActor, which is controlled by the player.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		AActor* GetTransformActor() const { return TransformActor; }


	/*Set the PreviousTransformActor that the player was controlling.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetPreviousTransformActor(AActor* InPreviousTransformActor) { PreviousTransformActor = InPreviousTransformActor; }
	/*Get PreviousTransformActor, which was managed by the player.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		AActor* GetPreviousTransformActor() const { return PreviousTransformActor; }


	/*Get the state of the actor through which you can select an operation on it.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		ETransformState GetTransformState() const { return TransformState; }
	/*Set the state of the actor, through which you can select an operation on it.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetTransformState(ETransformState InTransformState) { TransformState = InTransformState; }


	/*Saves the state of the actors: under the control of the player cursor or not.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetIsTransform(bool InIsTransform) { bIsTransform = InIsTransform; }
	/*The state of the actors: under the control of the player cursor or not.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		bool GetIsTransform() const { return bIsTransform; }


	/*Timer for moving actors.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		FTimerHandle GetLocationTimer() const { return LocationTimer; }
	/*Timer for rotation of actors.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		FTimerHandle GetRotationTimer() const { return RotationTimer; }
	/*Timer for scaling actors.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		FTimerHandle GetScaleTimer() const { return ScaleTimer; }

	/*The period when the timer for moving actors is operating.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetLocationTimerDeltaTime(float InLocationTimerDeltaTime) { LocationTimerDeltaTime = InLocationTimerDeltaTime; }
	/*The period when the timer for moving actors is operating.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetLocationTimerDeltaTime() { return LocationTimerDeltaTime; }
	/*The period when the timer for rotation actors is operating.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetRotationTimerDeltaTime(float InRotationTimerDeltaTime) { RotationTimerDeltaTime = InRotationTimerDeltaTime; }
	/*The period when the timer for rotation actors is operating.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetRotationTimerDeltaTime() { return RotationTimerDeltaTime; }
	/*The period when the timer for scale actors is operating.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetScaleTimerDeltaTime(float InScaleTimerDeltaTime) { ScaleTimerDeltaTime = InScaleTimerDeltaTime; }
	/*The period when the timer for scale actors is operating.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetScaleTimerDeltaTime() { return ScaleTimerDeltaTime; }

	/*Blocking actions in the first tick of the timer.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		bool GetIsLockFirstIterationLocationTimer() const { return bIsLockFirstIterationLocationTimer; }
	/*Blocking actions in the first tick of the timer.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetIsLockFirstIterationLocationTimer(bool InIsLockFirstIterationLocationTimer) { bIsLockFirstIterationLocationTimer = InIsLockFirstIterationLocationTimer; }
	/*Blocking actions in the first tick of the timer.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		bool GetIsLockFirstIterationRotationTimer() const { return bIsLockFirstIterationRotationTimer; }
	/*Blocking actions in the first tick of the timer.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetIsLockFirstIterationRotationTimer(bool InIsLockFirstIterationRotationTimer) { bIsLockFirstIterationRotationTimer = InIsLockFirstIterationRotationTimer; }
	/*Blocking actions in the first tick of the timer.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		bool GetIsLockFirstIterationScaleTimer() const { return bIsLockFirstIterationScaleTimer; }
	/*Blocking actions in the first tick of the timer.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetIsLockFirstIterationScaleTimer(bool InIsLockFirstIterationScaleTimer) { bIsLockFirstIterationScaleTimer = InIsLockFirstIterationScaleTimer; }



	/*Get the Player Controller you're using.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		APlayerController* GetPlayerController() const { return PlayerController; }
	/*Specify Player Controller for use by the component.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetPlayerController(APlayerController* InPlayerController) { PlayerController = InPlayerController; }


	/*Get the Players Pawn you are using.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		APawn* GetPlayerPawn() const { return PlayerPawn; }
	/*Specify Players Pawn for use by the component.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetPlayerPawn(APawn* InPlayerPawn) { PlayerPawn = InPlayerPawn; }


	/*Saving mouse coordinates when clicking on TransformActor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetLocationXAtClick(float InLocationXAtClick) { LocationXAtClick = InLocationXAtClick; }
	/*Saving mouse coordinates when clicking on TransformActor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetLocationXAtClick() const { return LocationXAtClick; }
	/*Saving mouse coordinates when clicking on TransformActor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetLocationYAtClick(float InLocationYAtClick) { LocationYAtClick = InLocationYAtClick; }
	/*Saving mouse coordinates when clicking on TransformActor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetLocationYAtClick() const { return LocationYAtClick; }


	/*Saving the distance from the mouse cursor to TransformActor, which was found when you first click on TransformActor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetDistanceToCursorSave(float InDistanceToCursorSave) { DistanceToCursorSave = InDistanceToCursorSave; }
	/*Get the distance from the mouse cursor to TransformActor, which was found when you first click on TransformActor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetDistanceToCursorSave() const { return DistanceToCursorSave; }


	/*Save scale when you click on TransformActor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetScale3DSave(FVector InScale3DSave) { Scale3DSave = InScale3DSave; }
	/*Get scale that when you click on TransformActor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		FVector GetScale3DSave() const { return Scale3DSave; }
	/*New scale TransformActor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetNewScale3D(FVector InNewScale3D) { NewScale3D = InNewScale3D; }
	/*New scale TransformActor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		FVector GetNewScale3D() const { return NewScale3D; }


	/*The difference between the rotation degrees in the current and previous call of the SetDeltaRoll, SetDeltaPitch, SetDeltaYaw methods.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetDeltaRollDegree() const { return DeltaRollDegree; }
	/*The difference between the rotation degrees in the current and previous call of the SetDeltaRoll, SetDeltaPitch, SetDeltaYaw methods.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetDeltaPitchDegree() const { return DeltaPitchDegree; }
	/*The difference between the rotation degrees in the current and previous call of the SetDeltaRoll, SetDeltaPitch, SetDeltaYaw methods.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetDeltaYawDegree() const { return DeltaYawDegree; }

	/*Set the saved values that will be used as starting positions of the mouse cursor when you click on the actor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetRollSave(float InRollSave) { RollSave = InRollSave; }
	/*Set the saved values that will be used as starting positions of the mouse cursor when you click on the actor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetRollSave() const { return RollSave; }
	/*Set the saved values that will be used as starting positions of the mouse cursor when you click on the actor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetPitchSave(float InPitchSave) { PitchSave = InPitchSave; }
	/*Set the saved values that will be used as starting positions of the mouse cursor when you click on the actor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetPitchSave() const { return PitchSave; }
	/*Set the saved values that will be used as starting positions of the mouse cursor when you click on the actor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetYawSave(float InYawSave) { YawSave = InYawSave; }
	/*Set the saved values that will be used as starting positions of the mouse cursor when you click on the actor.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetYawSave() const { return YawSave; }


	/*Parameter for VInterpConstantTo, interpolation speed of the relocation vector.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetLocationSpeed(float InLocationSpeed) { LocationSpeed = InLocationSpeed; }
	/*Parameter for VInterpConstantTo, interpolation speed of the relocation vector.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetLocationSpeed() const { return LocationSpeed; }
	/*If true than actor under cursor can't move through other objects . If false than actor can do it*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetSweep(bool InSweep) { bSweep = InSweep; }
	/*If true than actor under cursor can't move through other objects . If false than actor can do it*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		bool GetSweep() const { return bSweep; }
	/*Scaling speed.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetScaleSpeed(float InScaleSpeed) { ScaleSpeed = InScaleSpeed; }
	/*Scaling speed.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetScaleSpeed() const { return ScaleSpeed; }
	/*Speed of rotation.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		void SetRotationSpeed(float InRotationSpeed) { RotationSpeed = InRotationSpeed; }
	/*Speed of rotation.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetRotationSpeed() const { return RotationSpeed; }


	/*The sum of AxisValue values.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetSumInputAxisValue(float InSumInputAxisValue) { SumInputAxisValue = InSumInputAxisValue; }
	/*The sum of AxisValue values.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetSumInputAxisValue() const { return SumInputAxisValue; }


	/*The component from which the axes of transformation are taken.
	Now ComponentForTransformationAxis use for
	rotation with cursor,
	translation with keyboard and rotation with keyboard.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetComponentForTransformationAxis(USceneComponent* InComponentForTransformationAxis) { ComponentForTransformationAxis = InComponentForTransformationAxis; }
	/*The component from which the axes of transformation are taken.
	Now ComponentForTransformationAxis use for
	rotation with cursor,
	translation with keyboard and rotation with keyboard.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		USceneComponent* GetComponentForTransformationAxis() const { return ComponentForTransformationAxis; }


	/*The speed of movement to the depth (from yourself or to yourself).*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetLocationDeepSpeed(float InLocationDeepSpeed) { LocationDeepSpeed = InLocationDeepSpeed; }
	/*The speed of movement to the depth (from yourself or to yourself).*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetLocationDeepSpeed() const { return LocationDeepSpeed; }


	/*Minimum scale with cursor and keyboard.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Setters")
		void SetMinScale(float InMinScale) { MinScale = InMinScale; }
	/*Minimum scale with cursor and keyboard.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Getters")
		float GetMinScale() const { return MinScale; }


	/*
	*	Getters and Setters for bool variables that show transformation status and other variables with use keyboard.
	*/

	/*Set transformation status: Location left or right.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Setters")
		void SetIsLocationLeftRightKeyboard(bool InIsLocationLeftRightKeyboard) { bIsLocationLeftRightKeyboard = InIsLocationLeftRightKeyboard; }
	/*Show transformation status: Location left or right.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Getters")
		bool GetIsLocationLeftRightKeyboard() const { return bIsLocationLeftRightKeyboard; }
	/*Set transformation status: Location up or down.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Setters")
		void SetIsLocationUpDownKeyboard(bool InIsLocationUpDownKeyboard) { bIsLocationUpDownKeyboard = InIsLocationUpDownKeyboard; }
	/*Show transformation status: Location up or down.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Getters")
		bool GetIsLocationUpDownKeyboard() const { return bIsLocationUpDownKeyboard; }
	/*Set transformation status: Location inside or outside.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Setters")
		void SetIsLocationInsideOutsideKeyboard(bool InIsLocationInsideOutsideKeyboard) { bIsLocationInsideOutsideKeyboard = InIsLocationInsideOutsideKeyboard; }
	/*Show transformation status: Location inside or outside.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Getters")
		bool GetIsLocationInsideOutsideKeyboard() const { return bIsLocationInsideOutsideKeyboard; }
	/*Speed of relocation actor with keyboard.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Setters")
		void SetLocationSpeedKeyboard(float InLocationSpeedKeyboard) { LocationSpeedKeyboard = InLocationSpeedKeyboard; }
	/*Speed of relocation actor with keyboard.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Getters")
		float GetLocationSpeedKeyboard() const { return LocationSpeedKeyboard; }


	/*Set transformation status: Rotation Roll.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Setters")
		void SetIsRotationRollKeyboard(bool InIsRotationRollKeyboard) { bIsRotationRollKeyboard = InIsRotationRollKeyboard; }
	/*Show transformation status: Rotation Roll.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Getters")
		bool GetIsRotationRollKeyboard() const { return bIsRotationRollKeyboard; }
	/*Set transformation status: Rotation Pitch.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Setters")
		void SetIsRotationPitchKeyboard(bool InIsRotationPitchKeyboard) { bIsRotationPitchKeyboard = InIsRotationPitchKeyboard; }
	/*Show transformation status: Rotation Pitch.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Getters")
		bool GetIsRotationPitchKeyboard() const { return bIsRotationPitchKeyboard; }
	/*Set transformation status: Rotation Yaw.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Setters")
		void SetIsRotationYawKeyboard(bool InIsRotationYawKeyboard) { bIsRotationYawKeyboard = InIsRotationYawKeyboard; }
	/*Show transformation status: Rotation Yaw.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Getters")
		bool GetIsRotationYawKeyboard() const { return bIsRotationYawKeyboard; }
	/*Speed of rotation actor with keyboard.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Setters")
		void SetRotationSpeedKeyboard(float InRotationSpeedKeyboard) { RotationSpeedKeyboard = InRotationSpeedKeyboard; }
	/*Speed of rotation actor with keyboard.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Getters")
		float GetRotationSpeedKeyboard() const { return RotationSpeedKeyboard; }


	/*Set transformation status: Scale.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Setters")
		void SetIsScaleKeyboard(bool InIsScaleKeyboard) { bIsScaleKeyboard = InIsScaleKeyboard; }
	/*Show transformation status: Scale.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Getters")
		bool GetIsScaleKeyboard() const { return bIsScaleKeyboard; }
	/*Set transformation status: Scale X.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Setters")
		void SetIsScaleXKeyboard(bool InIsScaleXKeyboard) { bIsScaleXKeyboard = InIsScaleXKeyboard; }
	/*Show transformation status: Scale X.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Getters")
		bool GetIsScaleXKeyboard() const { return bIsScaleXKeyboard; }
	/*Set transformation status: Scale Y.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Setters")
		void SetIsScaleYKeyboard(bool InIsScaleYKeyboard) { bIsScaleYKeyboard = InIsScaleYKeyboard; }
	/*Show transformation status: Scale Y.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Getters")
		bool GetIsScaleYKeyboard() const { return bIsScaleYKeyboard; }
	/*Set transformation status: Scale Z.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Setters")
		void SetIsScaleZKeyboard(bool InIsScaleZKeyboard) { bIsScaleZKeyboard = InIsScaleZKeyboard; }
	/*Show transformation status: Scale Z.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Getters")
		bool GetIsScaleZKeyboard() const { return bIsScaleZKeyboard; }
	/*Speed of scale actor with keyboard.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Setters")
		void SetScaleSpeedKeyboard(float InScaleSpeedKeyboard) { ScaleSpeedKeyboard = InScaleSpeedKeyboard; }
	/*Speed of scale actor with keyboard.*/
	UFUNCTION(BlueprintCallable, Category = "TransformationActorsComponent | Keyboard | Getters")
		float GetScaleSpeedKeyboard() const { return ScaleSpeedKeyboard; }






};
