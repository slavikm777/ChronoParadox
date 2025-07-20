// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPTimeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONOPARADOX_API UCPTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPTimeComponent();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float _recordingFrameInterval = 0.03f;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	int32 _maxRecordingTime = 10;

	FTimerHandle _timerHandleRecording;
	bool _isRecording = false;
	bool _isPlayingRecord = false;
	int32 _currentFrameIndex = 0;
	FTimerHandle _timerHandlePlayingRecording;
	bool _removeFrame = false;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
