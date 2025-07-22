#pragma once

#include "CoreMinimal.h"

class CHRONOPARADOX_API CPHelperFunctions
{
public:
	template <class T>
	static T* GetComponentByInterface(const AActor* Actor)
	{
		if (Actor)
		{
			const TArray<UActorComponent*> ActorComponents = Actor->GetComponentsByInterface(T::UClassType::StaticClass());
			if (ActorComponents.Num() > 0)
			{
				return Cast<T>(ActorComponents[0]);
			}
			checkNoEntry();
		}
		return nullptr;
	}

	template <class T>
	static TArray<T*> GetComponentsByInterface(const AActor* Actor)
	{
		TArray<T*> InterfacesComponent;
		if (Actor)
		{
			const TArray<UActorComponent*> ActorComponents = Actor->GetComponentsByInterface(T::UClassType::StaticClass());
			for (UActorComponent* ActorComponent : ActorComponents)
			{
				InterfacesComponent.Add(Cast<T>(ActorComponent));
			}
		}
		return InterfacesComponent;
	}
};
