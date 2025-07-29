// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChronoParadox/ChronoParadoxCharacter.h"
#include "Components/ActorComponent.h"
#include "ChronoParadox/Core/CPCoreTypes.h"
#include "CPTimeParamInterface.h"
#include "CPAnimInterface.h"
#include "CPTimeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToggleReverse, bool, Active);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONOPARADOX_API UCPTimeComponent : public UActorComponent, public ICPTimeParamInterface
{
	GENERATED_BODY()

public:
	UCPTimeComponent();

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnToggleReverse OnToggleReversEvent;

protected:
	virtual void BeginPlay() override;
	virtual void UpdateAnimation(FAnimInfo NewAnim) override;
	virtual void ToggleReverse(bool Reverse) override;
	virtual void ToggleStopTime(bool StopTime) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float _recordingFrameInterval = 0.03f;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	int32 _maxRecordingTime = 15;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool _removeFrame = false;
	
	FTimerHandle _timerHandleRecording;
	bool _isRecording = false;
	bool _isPlayingRecord = false;
	int32 _currentFrameIndex = 0;
	FTimerHandle _timerHandlePlayingRecording;
	TArray<FFrameInfo> _recordFrames;

	UFUNCTION(BlueprintPure)
	FAnimInfo& GetCurrentAnimation()
	{
		return _currentAnimation;
	}

private:
	UPROPERTY()
	TObjectPtr<AChronoParadoxCharacter> _character = nullptr;
	ICPAnimInterface *_animInterface = nullptr;
	FAnimInfo _currentAnimation;
	UPROPERTY()
	TObjectPtr<AActor> _owner;
	
	void StartRecord();
	void FrameRecord();
	void StopRecord();
	void StartPlayingRecordReverse();
	void PlayingRecordReverse();
	void StopPlayingRecord();
};
