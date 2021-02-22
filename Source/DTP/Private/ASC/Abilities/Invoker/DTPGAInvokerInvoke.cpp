// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/Abilities/Invoker/DTPGAInvokerInvoke.h"

#include "Characters/DTPCharacterInvoker.h"

void UDTPGAInvokerInvoke::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Invoke();
}

TArray<InvokerReagents> UDTPGAInvokerInvoke::TransformReagentAttributesToEnumArray()
{
	TArray<InvokerReagents> OutArray;
	
	float FirstReagentValue = GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(
		UDTPAttributeSetInvoker::GetFirstReagentAttribute());
	float SecondReagentValue = GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(
		UDTPAttributeSetInvoker::GetSecondReagentAttribute());
	float ThirdReagentValue = GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(
		UDTPAttributeSetInvoker::GetThirdReagentAttribute());

	OutArray.Add(TransformSingleReagentAttributeToEnum(FirstReagentValue));
	OutArray.Add(TransformSingleReagentAttributeToEnum(SecondReagentValue));
	OutArray.Add(TransformSingleReagentAttributeToEnum(ThirdReagentValue));

	return OutArray;
}

InvokerReagents UDTPGAInvokerInvoke::TransformSingleReagentAttributeToEnum(float ReagentValue)
{
	if (static_cast<int>(ReagentValue) == InvokerReagents::Quax)
	{
		return InvokerReagents::Quax;
	}
	else if (static_cast<int>(ReagentValue) == InvokerReagents::Wex)
	{
		return InvokerReagents::Wex;
	}
	else if (static_cast<int>(ReagentValue) == InvokerReagents::Exort)
	{
		return InvokerReagents::Exort;
	}

	return InvokerReagents::None;
}

void UDTPGAInvokerInvoke::Invoke()
{
	FReagentOrderInfo ReagentOrderInfo;
	
	ReagentOrderInfo.Reagents.Append(TransformReagentAttributesToEnumArray());

	TSubclassOf<UGameplayAbility> AbilityToGrant = ReagentOrderInfo.TransformReagentsToAbilityClassToGrant(
		AbilityNameAndAbilityClassMap);

	if (AbilityToGrant == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combination of reagents returned no valid ability to grant"));
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
			true, true);
		return;
	}

	ADTPCharacterInvoker* InvokerCharacter = Cast<ADTPCharacterInvoker>(GetAvatarActorFromActorInfo());
	if (InvokerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No InvokerCharacter @Invoke"));
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
			true, true);
		return;
	}
	// First check if the ability the reagents invoked is not already invoked and act accordingly
	if (AbilityToGrant == InvokerCharacter->FirstSlotAbility)
	{
		// If the ability we are trying to invoke is already on the first slot, do nothing
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
			true, true);
		return;
	}
	else if (AbilityToGrant == InvokerCharacter->SecondSlotAbility)
	{
		// If the ability we are trying to invoke is on the second slot, make FirstSlot and SecondSlot exchange places
		// but don't commit cooldown
		TSubclassOf<UGameplayAbility> TempFirstSlotAbility = InvokerCharacter->FirstSlotAbility;
		TSubclassOf<UGameplayAbility> TempSecondSlotAbility = InvokerCharacter->SecondSlotAbility;

		InvokerCharacter->FirstSlotAbility = TempSecondSlotAbility;
		InvokerCharacter->SecondSlotAbility = TempFirstSlotAbility;

		// TODO(Wuhie): UMG reactions to switching abilities
		if (InvokerCharacter->FirstSlotAbility != nullptr)
		{
			InvokerCharacter->FirstSlotChangedDelegate.Broadcast(InvokerCharacter->FirstSlotAbility->GetName());
		}
		if (InvokerCharacter->SecondSlotAbility != nullptr)
		{
			InvokerCharacter->SecondSlotChangedDelegate.Broadcast(InvokerCharacter->SecondSlotAbility->GetName());
		}
		
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
			true, true);
		return;
	}
	else
	{
		// Commit cooldown and cost
		CommitAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo());
		
		// Remove the ability in the SecondSlotAbility because it's dropped
		for (const FGameplayAbilitySpec& Spec: GetAbilitySystemComponentFromActorInfo()->GetActivatableAbilities())
		{
			if (Spec.Ability->GetClass() == InvokerCharacter->SecondSlotAbility)
			{
				GetAbilitySystemComponentFromActorInfo()->ClearAbility(Spec.Handle);
			}
		}

		// Switch first ability slot to the second slot
		InvokerCharacter->SecondSlotAbility = InvokerCharacter->FirstSlotAbility;

		// The ability invoked by reagents will be set to the FirstSlotAbility
		InvokerCharacter->FirstSlotAbility = AbilityToGrant;

		// TODO(Wuhie): UMG Reactions to FirstSlot and SecondSlot changes, namely change the icon of the available
		// ability
		if (InvokerCharacter->FirstSlotAbility != nullptr)
		{
			InvokerCharacter->FirstSlotChangedDelegate.Broadcast(InvokerCharacter->FirstSlotAbility->GetName());
		}
		if (InvokerCharacter->SecondSlotAbility != nullptr)
		{
			InvokerCharacter->SecondSlotChangedDelegate.Broadcast(InvokerCharacter->SecondSlotAbility->GetName());
		}

		// Grant the ability that was invoked by the reagents
		if (GetAvatarActorFromActorInfo()->GetLocalRole() == ROLE_Authority)
		{
			GetAbilitySystemComponentFromActorInfo()->GiveAbility(FGameplayAbilitySpec(AbilityToGrant));
		}
		
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
			true, false);
	}
}
