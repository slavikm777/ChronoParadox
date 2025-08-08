// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPTimeParamInterface.h"
#include "CPTimeControlComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONOPARADOX_API UCPTimeControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPTimeControlComponent();

	UFUNCTION(BlueprintCallable) //Тест
	void TimeReverse(bool Active);

	UFUNCTION(BlueprintCallable) //Тест
	void StartChronoEcho();

protected:
	virtual void BeginPlay() override;

private:
	ICPTimeParamInterface *CPTimeParamInterface;
	ICPTimeParamInterface *TimeInterface;
	TArray<AActor*> ActorsWithTime;

	void MultiSphereTraceByChannel(
		const FVector StartLocation,
		const FVector EndLocation,
		float Radius,
		TEnumAsByte<ECollisionChannel> TraceChannel,
		bool bTraceComplex,
		const TArray<AActor*>& ActorsToIgnore,
		bool bIgnoreSelf,
		bool bDrawDebug,
		TArray<FHitResult>& OutHits,
		bool bOutBlockingHit
	);
	
	void GetAllActorsWithTime(float Radius);
	
	UFUNCTION()
	void ReverseDiactivate(bool Active);
};
