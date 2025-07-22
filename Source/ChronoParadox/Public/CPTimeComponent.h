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

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ReverseTime(bool Reverse);

protected:
	virtual void BeginPlay() override;
	virtual void UpdateAnimation(FAnimInfo NewAnim) override;
	
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
	
	void StartRecord();
	void FrameRecord();
	void StopRecord();
	void StartPlayingRecord();
	void PlayingRecord();
	void StopPlayingRecord();
};
