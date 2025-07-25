// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/TimeAnimNotifyState.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimSequence.h" 
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

void UTimeAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp,UAnimSequenceBase* Animation,float TotalDuration,const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
    CurrentTime = 0;
    IAnimInstance = Cast<ICPAnimInterface>(MeshComp->GetAnimInstance());
}


void UTimeAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
    if (!MeshComp || !Animation) return;
    
    if (IAnimInstance)
    {
        const FFrameRate FrameRate = Animation->GetSamplingFrameRate();
        if (FrameRate.Numerator <= 0 || FrameRate.Denominator <= 0) return;

        const float AnimLength = Animation->GetPlayLength();
        float TotalFrame = FrameRate.Numerator * AnimLength;
        CurrentTime += (FrameDeltaTime / TotalFrame) + FrameDeltaTime;

        int32 FrameIndex = CurrentTime * FrameRate.Numerator;
        bool NewAnim = FrameIndex <= 1;
        IAnimInstance->SetAnimInfo(FAnimInfo(Animation, NewAnim ? 1.f : FrameIndex / TotalFrame, NewAnim));
    }
}

void UTimeAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);
    CurrentTime = 0;
    IAnimInstance = nullptr;
}
