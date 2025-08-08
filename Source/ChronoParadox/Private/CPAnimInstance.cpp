#include "CPAnimInstance.h"
#include "GameFramework/Character.h"
#include "ChronoParadox/Helper/CPHelperFunctions.h"

void UCPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	_character = Cast<ACharacter>(GetOwningActor());
	_componentTime = CPHelperFunctions::GetComponentByInterface<ICPTimeParamInterface>(_character);
}

void UCPAnimInstance::SetAnimInfo(FAnimInfo AnimInfo)
{
	if (_timeReverse)
		return;
	AnimationInformation.AnimSequence = AnimInfo.AnimSequence;
	AnimationInformation.Frame = AnimInfo.Frame;
	AnimationInformation.NewAnim = AnimInfo.NewAnim;
	if (_componentTime)
		_componentTime->UpdateAnimation(AnimationInformation);
}

UCPAnimInstance& UCPAnimInstance::GetAnimInstance()
{
	return *this;
}

void UCPAnimInstance::PlayAnim(FAnimInfo AnimInfo)
{
	if (AnimInfo.AnimSequence != nullptr)
		AnimationInfo = AnimInfo;
}

void UCPAnimInstance::ReverseAnim(bool Active)
{
	_timeReverse = Active;
}

void UCPAnimInstance::ReplayAnim(bool Active)
{
	_timeReplay = Active;
}
