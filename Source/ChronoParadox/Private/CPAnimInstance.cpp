#include "CPAnimInstance.h"

void UCPAnimInstance::SetFrame(int32 frame)
{
	_frame = frame;
}

void UCPAnimInstance::SetSequenceBase(UAnimSequenceBase* InSequence)
{
	_sequenceBase = InSequence;
}

UCPAnimInstance& UCPAnimInstance::GetAnimInstance()
{
	return *this;
}
