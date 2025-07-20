#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPAnimInterface.generated.h"

class UCPAnimInstance;
class UAnimSequenceBase;

UINTERFACE(MinimalAPI)
class UCPAnimInterface : public UInterface
{
	GENERATED_BODY()
};

class ICPAnimInterface
{
	GENERATED_BODY()

public:
	virtual void SetFrame(int32 frame) = 0;
	virtual void SetSequenceBase(UAnimSequenceBase* InSequence) = 0;
	virtual UCPAnimInstance& GetAnimInstance() = 0;
};
