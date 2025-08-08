// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CPTimeControlComponent.h"
#include "ChronoParadox/Helper/CPHelperFunctions.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CPTimeComponent.h"

UCPTimeControlComponent::UCPTimeControlComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCPTimeControlComponent::BeginPlay()
{
	Super::BeginPlay();
	CPTimeParamInterface = CPHelperFunctions::GetComponentByInterface<ICPTimeParamInterface>(GetOwner());
	check(CPTimeParamInterface);
	CPTimeParamInterface->OnToggleReverse().AddDynamic(this, &ThisClass::ReverseDiactivate);
}

void UCPTimeControlComponent::MultiSphereTraceByChannel(const FVector StartLocation, const FVector EndLocation,
                                                        float Radius, TEnumAsByte<ECollisionChannel> TraceChannel,
                                                        bool bTraceComplex,
                                                        const TArray<AActor*>& ActorsToIgnore, bool bIgnoreSelf,
                                                        bool bDrawDebug, TArray<FHitResult>& OutHits,
                                                        bool bOutBlockingHit)
{
	// Настройка игнорируемых акторов
	TArray<AActor*> IgnoreList = ActorsToIgnore;
	if (bIgnoreSelf)
	{
		IgnoreList.AddUnique(GetOwner());
	}

	// Параметры отладки
	EDrawDebugTrace::Type DebugDrawType = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	// Выполнение трассировок
	bOutBlockingHit = false;
	TArray<FHitResult> LocalHits;
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		StartLocation,
		EndLocation,
		Radius,
		UEngineTypes::ConvertToTraceType(TraceChannel.GetValue()), // Конвертация ECollisionChannel -> ETraceTypeQuery
		bTraceComplex,
		IgnoreList,
		DebugDrawType,
		LocalHits,
		true
	);

	if (bHit)
	{
		bOutBlockingHit = true;
		OutHits.Append(LocalHits);
	}
}

void UCPTimeControlComponent::GetAllActorsWithTime(float Radius)
{
	TArray<FHitResult> HitResults;
	FVector StartLocation = GetOwner()->GetActorLocation();
	FVector EndLocation = GetOwner()->GetActorForwardVector() * 10 + GetOwner()->GetActorLocation();
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Visibility;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(GetOwner());
	MultiSphereTraceByChannel(StartLocation, EndLocation, Radius, TraceChannel, false, ActorsToIgnore, true, false,
	                          HitResults, true);
	ActorsWithTime.Empty();
	if (HitResults.Num() > 0)
	{
		for (int i = 0; i < HitResults.Num(); i++)
			if (HitResults[i].GetActor()->GetComponentByClass(UCPTimeComponent::StaticClass()))
				ActorsWithTime.Add(HitResults[i].GetActor());
	}
}

void UCPTimeControlComponent::ReverseDiactivate(bool Active)
{
	if (Active == false && ActorsWithTime.Num()-1 > 0)
	{
		for (AActor *Actor : ActorsWithTime)
		{
			TimeInterface = CPHelperFunctions::GetComponentByInterface<ICPTimeParamInterface>(Actor);
			if (TimeInterface)
				TimeInterface->ToggleReverse(Active);
		}
		ActorsWithTime.Empty();
	}
}

void UCPTimeControlComponent::TimeReverse(bool Active)
{
	CPTimeParamInterface->ToggleReverse(Active);
	if (Active)
		GetAllActorsWithTime(1000000.f);
	if (ActorsWithTime.Num()-1 > 0)
		for (AActor *Actor : ActorsWithTime)
	{
		TimeInterface = CPHelperFunctions::GetComponentByInterface<ICPTimeParamInterface>(Actor);
		if (TimeInterface)
			TimeInterface->ToggleReverse(Active);
	}
}

void UCPTimeControlComponent::StartChronoEcho()
{
	CPTimeParamInterface->StartChronoEcho();
}
