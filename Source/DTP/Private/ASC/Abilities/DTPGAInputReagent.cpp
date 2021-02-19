// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/Abilities/DTPGAInputReagent.h"

#include "AbilitySystemComponent.h"
#include "ASC/AttributeSets/DTPAttributeSetInvoker.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UDTPGAInputReagent::UDTPGAInputReagent()
{

}

void UDTPGAInputReagent::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData * TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	UAbilityTask_WaitGameplayEvent* WaitGETaskQuax = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,
		FGameplayTag::RequestGameplayTag(FName("Event.Characters.Invoker.Reagent.Quax")));
	UAbilityTask_WaitGameplayEvent* WaitGETaskWex = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,
		FGameplayTag::RequestGameplayTag(FName("Event.Characters.Invoker.Reagent.Wex")));
	UAbilityTask_WaitGameplayEvent* WaitGETaskExort = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, FGameplayTag::RequestGameplayTag(FName("Event.Characters.Invoker.Reagent.Exort")));

	WaitGETaskExort->EventReceived.AddDynamic(this, &UDTPGAInputReagent::EventReceived);
	WaitGETaskQuax->EventReceived.AddDynamic(this, &UDTPGAInputReagent::EventReceived);
	WaitGETaskWex->EventReceived.AddDynamic(this, &UDTPGAInputReagent::EventReceived);

	WaitGETaskQuax->ReadyForActivation();
	WaitGETaskWex->ReadyForActivation();
	WaitGETaskExort->ReadyForActivation();
}

void UDTPGAInputReagent::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UDTPGAInputReagent::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UDTPGAInputReagent::EventReceived(FGameplayEventData EventData)
{
	if (EventData.EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Characters.Invoker.Reagent.Quax")))
	{
		ShiftActiveReagentQueue(InvokerReagents::Quax);
	}
	else if (EventData.EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Characters.Invoker.Reagent.Wex")))
	{
		ShiftActiveReagentQueue(InvokerReagents::Wex);
	}
	else if (EventData.EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Characters.Invoker.Reagent.Exort")))
	{
		ShiftActiveReagentQueue(InvokerReagents::Exort);
	}
}

void UDTPGAInputReagent::ShiftActiveReagentQueue(int ReagentNumber)
{
	// Get attributes
	FGameplayAttribute FirstReagentAttribute = UDTPAttributeSetInvoker::GetFirstReagentAttribute();
	FGameplayAttribute SecondReagentAttribute = UDTPAttributeSetInvoker::GetSecondReagentAttribute();
	FGameplayAttribute ThirdReagentAttribute = UDTPAttributeSetInvoker::GetThirdReagentAttribute();
	// Get Numeric Values
	float FirstReagent = GetAbilitySystemComponentFromActorInfo()->
		GetNumericAttribute(FirstReagentAttribute);
	float SecondReagent = GetAbilitySystemComponentFromActorInfo()->
		GetNumericAttribute(SecondReagentAttribute);
	
	// Shift FirstReagent to ReagentNumber, SecondReagent to First, ThirdReagent to Second
	ShiftReagentAttributeDuo(static_cast<float>(ReagentNumber), FirstReagentAttribute);
	ShiftReagentAttributeDuo(FirstReagent, SecondReagentAttribute);
	ShiftReagentAttributeDuo(SecondReagent, ThirdReagentAttribute);
}

void UDTPGAInputReagent::ShiftReagentAttributeDuo(float PreviousReagentValue, FGameplayAttribute NextReagentAttribute)
{
	// Create the effect
	UGameplayEffect* GEShiftReagentAttribute = NewObject<UGameplayEffect>(GetTransientPackage(),
		FName(TEXT("ReagentShift")));
	GEShiftReagentAttribute->DurationPolicy = EGameplayEffectDurationType::Instant;
	int32 Idx = GEShiftReagentAttribute->Modifiers.Num();
	GEShiftReagentAttribute->Modifiers.SetNum(Idx + 1);
	GEShiftReagentAttribute->StackingType = EGameplayEffectStackingType::None;
	
	FGameplayModifierInfo& InfoNextReagent = GEShiftReagentAttribute->Modifiers[Idx];
	InfoNextReagent.ModifierMagnitude = FScalableFloat(PreviousReagentValue);
	InfoNextReagent.ModifierOp = EGameplayModOp::Override;
	InfoNextReagent.Attribute = NextReagentAttribute;
	
	FGameplayEffectSpec* GESpec = new FGameplayEffectSpec(GEShiftReagentAttribute, {}, 1.0f);
	ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
		GESpec);
}


