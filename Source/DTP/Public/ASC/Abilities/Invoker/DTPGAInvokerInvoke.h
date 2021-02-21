// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASC/Abilities/DTPGABase.h"
#include "ASC/AttributeSets/DTPAttributeSetInvoker.h"
#include "DTPGAInvokerInvoke.generated.h"

/**
 * 
 */
// A map of the values of the three reagents attributes
// And a function that translates the values of the reagents to the ability invoke can grant
USTRUCT()
struct FReagentOrderInfo
{
	GENERATED_BODY()

public:
	// represent the reagents that are active on invoker in order
	TArray<InvokerReagents> Reagents;

	int NumberOfQuaxReagents = 0;
	int NumberOfWexReagents = 0;
	int NumberOfExortReagents = 0;

	TSubclassOf<UGameplayAbility> TransformReagentsToAbilityClassToGrant(TMap<FName,
		TSubclassOf<UGameplayAbility>> AbilityNameAndAbilityClassMap)
	{
		GetCountOfOrbs();
		if (NumberOfQuaxReagents == 1 && NumberOfWexReagents == 2)
		{
			TSubclassOf<UGameplayAbility>* Ability = AbilityNameAndAbilityClassMap.Find(FName("Tornado"));
			if (Ability == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("No Ability found @FReagentOrderInfo"));
				return nullptr;
			}
			
			return *Ability;
		}

		return nullptr;
	}

	void GetCountOfOrbs()
	{
		for (InvokerReagents Reagent : Reagents)
		{
			if (Reagent == InvokerReagents::Quax)
			{
				NumberOfQuaxReagents += 1;
			}
			else if (Reagent == InvokerReagents::Wex)
			{
				NumberOfWexReagents += 1;
			}
			else if (Reagent == InvokerReagents::Exort)
			{
				NumberOfExortReagents += 1;
			}
		}
	}
};

UCLASS()
class DTP_API UDTPGAInvokerInvoke : public UDTPGABase
{
	GENERATED_BODY()
	
public:

	void Invoke();

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	TMap<FName, TSubclassOf<UGameplayAbility>> AbilityNameAndAbilityClassMap;

	TArray<InvokerReagents> TransformReagentAttributesToEnumArray();

	InvokerReagents TransformSingleReagentAttributeToEnum(float ReagentValue);
};
