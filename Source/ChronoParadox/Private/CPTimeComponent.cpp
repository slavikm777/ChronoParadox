// Fill out your copyright notice in the Description page of Project Settings.


#include "CPTimeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UCPTimeComponent::UCPTimeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCPTimeComponent::BeginPlay()
{
	Super::BeginPlay();
	_character = Cast<AChronoParadoxCharacter>(GetOwner());
	_animInterface = Cast<ICPAnimInterface>(_character->GetMesh()->GetAnimInstance());
	check(_character);
	check(_animInterface);
	StartRecord();
}

void UCPTimeComponent::UpdateAnimation(FAnimInfo NewAnim)
{
	_currentAnimation = NewAnim;
}

void UCPTimeComponent::StartRecord()
{
	_isRecording = true;
	GetWorld()->GetTimerManager().SetTimer(_timerHandleRecording, this, &ThisClass::FrameRecord, _recordingFrameInterval, true);
}

void UCPTimeComponent::FrameRecord()
{
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, "Recording");
	FFrameInfo frameInfo;
	frameInfo.Location = _character->GetActorLocation();
	frameInfo.Rotation = _character->GetActorRotation();
	frameInfo.Velocity = _character->GetVelocity();
	frameInfo.AnimInfo = _currentAnimation;
	if (_recordFrames.Num() <= (_maxRecordingTime/_recordingFrameInterval))
	{
		_recordFrames.Add(frameInfo);
	}
	else
	{
		_recordFrames.RemoveAt(0);
		_recordFrames.Add(frameInfo);
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
		_animInterface->PlayAnim(_recordFrames[_currentFrameIndex].AnimInfo);
		_character->SetActorLocationAndRotation(_recordFrames[_currentFrameIndex].Location, _recordFrames[_currentFrameIndex].Rotation, true);
		_character->GetMovementComponent()->Velocity = _recordFrames[_currentFrameIndex].Velocity;
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
		_animInterface->ReverseAnim(true);
		StopRecord();
		StartPlayingRecord();
	}
	else
	{
		_animInterface->ReverseAnim(false);
		StopPlayingRecord();
		StartRecord();
	}
}

