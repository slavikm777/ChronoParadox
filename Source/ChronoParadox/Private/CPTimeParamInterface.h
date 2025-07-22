// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPTimeParamInterface.generated.h"

class UAnimSequenceBase;
struct FAnimInfo;

UINTERFACE(MinimalAPI)
class UCPTimeParamInterface : public UInterface
{
	GENERATED_BODY()
};


class ICPTimeParamInterface
{
	GENERATED_BODY()

public:
	virtual void UpdateAnimation(FAnimInfo NewAnim) = 0;
};
