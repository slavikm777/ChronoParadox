// Fill out your copyright notice in the Description page of Project Settings.


#include "CPTimeComponent.h"

UCPTimeComponent::UCPTimeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCPTimeComponent::BeginPlay()
{
	Super::BeginPlay();
	StartRecord();
}

void UCPTimeComponent::StartRecord()
{
	_isRecording = true;
	GetWorld()->GetTimerManager().SetTimer(_timerHandleRecording, this, &ThisClass::FrameRecord, _recordingFrameInterval, true);
}

void UCPTimeComponent::FrameRecord()
{
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, "Recording");
	FAnimInfo animInfo;
	animInfo.Location = GetOwner()->GetActorLocation();
	animInfo.Rotation = GetOwner()->GetActorRotation();
	animInfo.Velocity = GetOwner()->GetVelocity();
	if (_recordFrames.Num() <= (_maxRecordingTime/_recordingFrameInterval))
	{
		_recordFrames.Add(animInfo);
	}
	else
	{
		_recordFrames.RemoveAt(0);
		_recordFrames.Add(animInfo);
	}
}

void UCPTimeComponent::StopRecord()
{
	GetWorld()->GetTimerManager().ClearTimer(_timerHandleRecording);
	_isRecording = false;
	//_recordFrames.Empty();
}

void UCPTimeComponent::StartPlayingRecord()
{
	_currentFrameIndex = _recordFrames.Num() - 1;
	_isRecording = true;
	GetWorld()->GetTimerManager().SetTimer(_timerHandlePlayingRecording, this, &ThisClass::PlayingRecord, _recordingFrameInterval, true);
}

void UCPTimeComponent::PlayingRecord()
{
	if (_currentFrameIndex >= 0)
	{
		GetOwner()->SetActorLocationAndRotation(_recordFrames[_currentFrameIndex].Location, _recordFrames[_currentFrameIndex].Rotation);
		if (_removeFrame)
			_recordFrames.RemoveAt(_currentFrameIndex);
		_currentFrameIndex--;
	}
}

void UCPTimeComponent::StopPlayingRecord()
{
	_isRecording = false;
	GetWorld()->GetTimerManager().ClearTimer(_timerHandlePlayingRecording);
}

void UCPTimeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UCPTimeComponent::ReverseTime(bool Reverse)
{
	if (Reverse)
	{
		StopRecord();
		StartPlayingRecord();
	}
	else
	{
		StopPlayingRecord();
		StartRecord();
	}
}

