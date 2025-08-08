// Fill out your copyright notice in the Description page of Project Settings.

#include "CPTimeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ChronoParadox/Helper/CPHelperFunctions.h"
#include "Components/CapsuleComponent.h"

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
		check(_characterClass);
	}
	_owner = Cast<AActor>(GetOwner());
	if (_isClone == false)
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

void UCPTimeComponent::SetFrames(TArray<FFrameInfo> InFrames, int32 CurrentFrameIndex)
{
	_isClone = true;
	_recordFrames = InFrames;
	RemovePastFrames(CurrentFrameIndex);
	StartPlayingRecord(0);
}

void UCPTimeComponent::StartChronoEcho()
{
	if (_isPlayingRecord)
	{
		SpawnCharacter();
		StopPlayingRecord();
		OnToggleReversEvent.Broadcast(false);
		_recordFrames.Empty();
		if (_character)
			_animInterface->ReverseAnim(false);
		StartRecord();
	}
}

FOnToggleReverse& UCPTimeComponent::OnToggleReverse()
{
	return OnToggleReversEvent;
}

void UCPTimeComponent::StartRecord()
{
	_isRecording = true;
	GetWorld()->GetTimerManager().SetTimer(_timerHandleRecording, this, &ThisClass::FrameRecord,
	                                       _recordingFrameInterval, true);
}

void UCPTimeComponent::FrameRecord()
{
	FFrameInfo frameInfo;
	frameInfo.Location = _owner->GetActorLocation();
	frameInfo.Rotation = _owner->GetActorRotation();
	frameInfo.Velocity = _owner->GetVelocity();
	if (_character && _currentAnimation.AnimSequence)
		frameInfo.AnimInfo = _currentAnimation;
	if (_recordFrames.Num() <= (_maxRecordingTime / _recordingFrameInterval))
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
	_timerHandleRecording.Invalidate();
	_isRecording = false;
}

void UCPTimeComponent::StartPlayingRecordReverse()
{
	_currentFrameIndex = _recordFrames.Num() - 1;
	_isPlayingRecord = true;
	if (_character)
		_character->GetMovementComponent()->Velocity = _recordFrames[_currentFrameIndex].Velocity;
	else
	{
		TObjectPtr<UStaticMeshComponent> StaticMeshComponent = Cast<UStaticMeshComponent>(
			_owner->GetComponentByClass(UMeshComponent::StaticClass()));
		StaticMeshComponent->SetSimulatePhysics(false);
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	GetWorld()->GetTimerManager().SetTimer(_timerHandlePlayingRecording, this, &ThisClass::PlayingRecordReverse,
	                                       _recordingFrameInterval, true);
}

void UCPTimeComponent::PlayingRecordReverse()
{
	if (_currentFrameIndex >= 0)
	{
		if (_character)
			_animInterface->PlayAnim(_recordFrames[_currentFrameIndex].AnimInfo);

		_owner->SetActorLocationAndRotation(_recordFrames[_currentFrameIndex].Location,
		                                    _recordFrames[_currentFrameIndex].Rotation, true);
		
		_currentFrameIndex--;
	}
	else
		ToggleReverse(false);
}

void UCPTimeComponent::StopPlayingRecord()
{
	_isPlayingRecord = false;
	GetWorld()->GetTimerManager().ClearTimer(_timerHandlePlayingRecording);
	_timerHandlePlayingRecording.Invalidate();
	
	if (_removeFrame && _recordFrames.Num() - 1 > 0)
		RemoveFutureFrames(_currentFrameIndex);

	if (_character)
	{
		if (_recordFrames.Num() - 1 > 0)
			_character->GetMovementComponent()->Velocity = _recordFrames[_recordFrames.Num() - 1].Velocity;
	}
	else
	{
		TObjectPtr<UStaticMeshComponent> StaticMeshComponent = Cast<UStaticMeshComponent>(_owner->GetComponentByClass(UMeshComponent::StaticClass()));
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		StaticMeshComponent->SetSimulatePhysics(true);
		if (_recordFrames.Num() - 1 > 0)
			StaticMeshComponent->SetPhysicsLinearVelocity(_recordFrames[_recordFrames.Num() - 1].Velocity, true);
	}
}

void UCPTimeComponent::StartPlayingRecord(int32 FrmeIndex)
{
	_currentFrameIndex = FrmeIndex;
	_isPlayingRecord = true;
	UE_LOG(LogTemp, Display, TEXT("StartFrameLength = %d"), _recordFrames.Num()-1);
	GetWorld()->GetTimerManager().SetTimer(_timerHandlePlayingRecording, this,
	                                       &ThisClass::PlayingRecord, _recordingFrameInterval, true);
}

void UCPTimeComponent::PlayingRecord()
{
	if (_currentFrameIndex <= _recordFrames.Num() - 1)
	{
		if (_character)
			_animInterface->PlayAnim(_recordFrames[_currentFrameIndex].AnimInfo);

		_owner->SetActorLocationAndRotation(_recordFrames[_currentFrameIndex].Location,
		                                    _recordFrames[_currentFrameIndex].Rotation, true);

		if (_character)
			_character->GetMovementComponent()->Velocity = _recordFrames[_currentFrameIndex].Velocity;
		else
		{
			TObjectPtr<UStaticMeshComponent> StaticMeshComponent = Cast<UStaticMeshComponent>(
				_owner->GetComponentByClass(UMeshComponent::StaticClass()));
			StaticMeshComponent->SetSimulatePhysics(false);
			StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		if (_character == nullptr && _recordFrames.Num() - 1 > 1)
			_recordFrames.RemoveAt(_currentFrameIndex);
		_currentFrameIndex++;
	}
	else
	{
		_isPlayingRecord = false;
		GetWorld()->GetTimerManager().ClearTimer(_timerHandlePlayingRecording);
		_timerHandlePlayingRecording.Invalidate();
		_character->Destroy();
	}
}

void UCPTimeComponent::RemoveFutureFrames(int32 FrmeIndex)
{
	if (_recordFrames.Num() - 1 > 0 && _recordFrames.Num() > FrmeIndex)
		for (int32 i = _recordFrames.Num() - 1; i > FrmeIndex; i--)
			_recordFrames.RemoveAt(i);
}

void UCPTimeComponent::RemovePastFrames(int32 FrmeIndex)
{
	if (_recordFrames.Num() - 1 > 0)
		for (int32 i = FrmeIndex; i > 1; i--)
			_recordFrames.RemoveAt(i);
}

bool UCPTimeComponent::SpawnCharacter()
{
	if (!GetWorld()) return false;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector SpawnLocation = _character->GetActorLocation();
	FRotator SpawnRotation = _character->GetActorRotation();
	AChronoParadoxCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<AChronoParadoxCharacter>(
		_characterClass,
		FTransform(SpawnRotation, SpawnLocation),
		nullptr, // Owner
		nullptr, // Instigator
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	if (NewCharacter)
	{
		ICPTimeParamInterface* newCharacterTime = CPHelperFunctions::GetComponentByInterface<ICPTimeParamInterface>(NewCharacter);
		ICPAnimInterface* newAnimInterface = Cast<ICPAnimInterface>(NewCharacter->GetMesh()->GetAnimInstance());
		check(newAnimInterface);
		check(newCharacterTime);
		newAnimInterface->ReplayAnim(true);
		newCharacterTime->SetFrames(_recordFrames, _currentFrameIndex);
		NewCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NewCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NewCharacter->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
	}
	else
	{
		return false;
	}
	return true;
}
