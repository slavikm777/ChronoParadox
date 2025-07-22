#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPAnimInterface.h"
#include "CPTimeParamInterface.h"
#include "ChronoParadox/Core/CPCoreTypes.h"
#include "CPAnimInstance.generated.h"

class ACharacter;

UCLASS()
class CHRONOPARADOX_API UCPAnimInstance : public UAnimInstance, public ICPAnimInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	bool _timeReverse = false;

	UPROPERTY(BlueprintReadOnly)
	FAnimInfo AnimationInfo;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> _character = nullptr;

	ICPTimeParamInterface* _componentTime = nullptr;
	
	virtual void NativeInitializeAnimation() override;

protected:
	virtual void SetAnimInfo(FAnimInfo AnimInfo) override;
	virtual UCPAnimInstance& GetAnimInstance() override;
	virtual void PlayAnim(FAnimInfo AnimInfo) override;
	virtual void ReverseAnim(bool Active) override;

private:
	FAnimInfo AnimationInformation;
};
