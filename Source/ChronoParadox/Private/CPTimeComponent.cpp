// Fill out your copyright notice in the Description page of Project Settings.


#include "CPTimeComponent.h"

UCPTimeComponent::UCPTimeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCPTimeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCPTimeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

