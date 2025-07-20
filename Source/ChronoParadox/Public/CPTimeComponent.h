// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChronoParadox/Core/CPCoreTypes.h"
#include "CPTimeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONOPARADOX_API UCPTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPTimeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ReverseTime(bool Reverse);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float _recordingFrameInterval = 0.03f;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	int32 _maxRecordingTime = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	bool _removeFrame = false;
	
	FTimerHandle _timerHandleRecording;
	bool _isRecording = false;
	bool _isPlayingRecord = false;
	int32 _currentFrameIndex = 0;
	FTimerHandle _timerHandlePlayingRecording;
	TArray<FAnimInfo> _recordFrames;

private:
	void StartRecord();
	void FrameRecord();
	void StopRecord();
	void StartPlayingRecord();
	void PlayingRecord();
	void StopPlayingRecord();
};
