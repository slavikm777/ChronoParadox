#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPAnimInterface.h"
#include "CPAnimInstance.generated.h"

UCLASS()
class CHRONOPARADOX_API UCPAnimInstance : public UAnimInstance, public ICPAnimInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	float _frame;

	UPROPERTY(BlueprintReadOnly)
	UAnimSequenceBase* _sequenceBase;
	
	UPROPERTY(BlueprintReadOnly)
	bool _timeReverse = false;

protected:
	virtual void SetFrame(int32 frame) override;
	virtual void SetSequenceBase(UAnimSequenceBase* InSequence) override;
	virtual UCPAnimInstance& GetAnimInstance() override;
};
