// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPCoreTypes.generated.h"

USTRUCT(Blueprintable)
struct FAnimInfo
{
	GENERATED_BODY()

	FAnimInfo()
		: Frame(0)
	    , AnimSequence(nullptr)
	    , NewAnim(true)
	{}

	FAnimInfo(UAnimSequenceBase *InAnimSequence, float InFrame, bool InNewAnim)
	{
		AnimSequence = InAnimSequence;
		Frame = InFrame;
		NewAnim = InNewAnim;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimSequenceBase *AnimSequence;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Frame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool NewAnim;
};

USTRUCT(BlueprintType)
struct FFrameInfo
{
	GENERATED_BODY()
	
	FFrameInfo()
		: AnimInfo(FAnimInfo())
		, Location(FVector::ZeroVector)
		, Rotation(FRotator::ZeroRotator)
		, Velocity(FVector::ZeroVector)
	{}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FAnimInfo AnimInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector Location;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRotator Rotation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector Velocity;
};
