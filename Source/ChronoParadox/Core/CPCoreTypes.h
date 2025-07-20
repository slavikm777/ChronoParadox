// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPCoreTypes.generated.h"

USTRUCT(BlueprintType)
struct FAnimInfo
{
	GENERATED_BODY()
	
	FAnimInfo()
		: Frame(0)
		, AnimSequence(nullptr)
		, Location(FVector::ZeroVector)
		, Rotation(FRotator::ZeroRotator)
		, Velocity(FVector::ZeroVector)
	{}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Frame;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimSequenceBase* AnimSequence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector Location;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRotator Rotation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector Velocity;
};
