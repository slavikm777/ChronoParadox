// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChronoParadox/ChronoParadoxCharacter.h"
#include "Components/ActorComponent.h"
#include "ChronoParadox/Core/CPCoreTypes.h"
#include "CPTimeParamInterface.h"
#include "CPAnimInterface.h"
#include "CPTimeComponent.generated.h"

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
	virtual void SetFrames(TArray<FFrameInfo> InFrames, int32 CurrentFrameIndex) override;
	virtual void StartChronoEcho() override;
	virtual FOnToggleReverse &OnToggleReverse() override;
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	float _recordingFrameInterval = 0.02f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	int32 _maxRecordingTime = 15;

	//UPROPERTY(EditAnywhere, Category = "Settings")
	bool _removeFrame = true;

private:
	FTimerHandle _timerHandleRecording;
	FTimerHandle _timerHandlePlayingRecording;
	bool _isRecording = false;
	bool _isPlayingRecord = false;
	bool _isClone = false;
	int32 _currentFrameIndex = 0;
	TArray<FFrameInfo> _recordFrames;
	ICPAnimInterface *_animInterface = nullptr;
	FAnimInfo _currentAnimation;
	
	UPROPERTY()
	TObjectPtr<AChronoParadoxCharacter> _character = nullptr;
	
	UPROPERTY()
	TObjectPtr<AActor> _owner;
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<class AChronoParadoxCharacter> _characterClass;
	
	void StartRecord();
	void FrameRecord();
	void StopRecord();
	void StartPlayingRecordReverse();
	void PlayingRecordReverse();
	void StopPlayingRecord();
	void StartPlayingRecord(int32 FrmeIndex = 0);
	void PlayingRecord();
	void RemoveFutureFrames(int32 FrmeIndex);
	void RemovePastFrames(int32 FrmeIndex);
	bool SpawnCharacter();
};
