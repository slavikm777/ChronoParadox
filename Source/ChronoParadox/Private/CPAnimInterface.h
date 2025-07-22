#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPAnimInterface.generated.h"

class UCPAnimInstance;
class UAnimSequenceBase;
struct FAnimInfo;

UINTERFACE(MinimalAPI)
class UCPAnimInterface : public UInterface
{
	GENERATED_BODY()
};

class ICPAnimInterface
{
	GENERATED_BODY()

public:
	virtual void SetAnimInfo(FAnimInfo AnimInfo) = 0;
	virtual UCPAnimInstance& GetAnimInstance() = 0;
	virtual void PlayAnim(FAnimInfo AnimInfo) = 0;
	virtual void ReverseAnim(bool Active) = 0;
};
