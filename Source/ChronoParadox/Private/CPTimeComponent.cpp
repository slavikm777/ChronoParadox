// Fill out your copyright notice in the Description page of Project Settings.


#include "CPTimeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"

UCPTimeComponent::UCPTimeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCPTimeComponent::BeginPlay()
{
	Super::BeginPlay();
	_character = Cast<AChronoParadoxCharacter>(GetOwner());
	if (_character)
	{
		_animInterface = Cast<ICPAnimInterface>(_character->GetMesh()->GetAnimInstance());
		check(_character);
		check(_animInterface);
	}
	_owner = Cast<AActor>(GetOwner());
	StartRecord();
}

void UCPTimeComponent::UpdateAnimation(FAnimInfo NewAnim)
{
	_currentAnimation = NewAnim;
}

void UCPTimeComponent::ToggleReverse(bool Reverse)
{
	if (Reverse)
	{
		if (_character)
			_animInterface->ReverseAnim(true);
		StopRecord();
		StartPlayingRecordReverse();
	}
	else
	{
		if (_character)
			_animInterface->ReverseAnim(false);
		StopPlayingRecord();
		StartRecord();
	}
	OnToggleReversEvent.Broadcast(Reverse);
}

void UCPTimeComponent::ToggleStopTime(bool StopTime)
{
}

void UCPTimeComponent::StartRecord()
{
	_isRecording = true;
	GetWorld()->GetTimerManager().SetTimer(_timerHandleRecording, this, &ThisClass::FrameRecord, _recordingFrameInterval, true);
}

void UCPTimeComponent::FrameRecord()
{
	FFrameInfo frameInfo;
	frameInfo.Location = _owner->GetActorLocation();
	frameInfo.Rotation = _owner->GetActorRotation();
	frameInfo.Velocity = _owner->GetVelocity();
	if (_character)
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

void UCPTimeComponent::StartPlayingRecordReverse()
{
	_currentFrameIndex = _recordFrames.Num() - 1;
	_isRecording = true;
	GetWorld()->GetTimerManager().SetTimer(_timerHandlePlayingRecording, this, &ThisClass::PlayingRecordReverse, _recordingFrameInterval, true);
}

void UCPTimeComponent::PlayingRecordReverse()
{
	if (_currentFrameIndex >= 0)
	{
		if (_character)
			_animInterface->PlayAnim(_recordFrames[_currentFrameIndex].AnimInfo);
		
		_owner->SetActorLocationAndRotation(_recordFrames[_currentFrameIndex].Location, _recordFrames[_currentFrameIndex].Rotation, true);
		
		if (_character)
			_character->GetMovementComponent()->Velocity = _recordFrames[_currentFrameIndex].Velocity;
		else
		{
			TObjectPtr<UStaticMeshComponent> StaticMeshComponent = Cast<UStaticMeshComponent>(_owner->GetComponentByClass(UMeshComponent::StaticClass()));
			//StaticMeshComponent->SetPhysicsLinearVelocity(_recordFrames[_currentFrameIndex].Velocity, true);
			StaticMeshComponent->SetSimulatePhysics(false);
			StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		if (_removeFrame && _currentFrameIndex >= 1)
			_recordFrames.RemoveAt(_currentFrameIndex);
		_currentFrameIndex--;
	}
	else
		ToggleReverse(false);
}

void UCPTimeComponent::StopPlayingRecord()
{
	if (_character)
	{
		_character->GetMovementComponent()->Velocity = _recordFrames[_recordFrames.Num()-1].Velocity;
	}
	else
	{
		TObjectPtr<UStaticMeshComponent> StaticMeshComponent = Cast<UStaticMeshComponent>(_owner->GetComponentByClass(UMeshComponent::StaticClass()));
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		StaticMeshComponent->SetSimulatePhysics(true);
		StaticMeshComponent->SetPhysicsLinearVelocity(_recordFrames[_recordFrames.Num()-1].Velocity, true);
	}
	_isRecording = false;
	GetWorld()->GetTimerManager().ClearTimer(_timerHandlePlayingRecording);
}
