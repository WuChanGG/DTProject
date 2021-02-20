// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/Abilities/DTPGAInputReagent.h"

#include "AbilitySystemComponent.h"
#include "ASC/AttributeSets/DTPAttributeSetInvoker.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Kismet/KismetSystemLibrary.h"

UDTPGAInputReagent::UDTPGAInputReagent()
{
	FAbilityTriggerData WexTriggerData = FAbilityTriggerData();
	WexTriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	WexTriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(FName("Event.Characters.Invoker.Reagent.Wex"));
	
	FAbilityTriggerData QuaxTriggerData = FAbilityTriggerData();
	QuaxTriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	QuaxTriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(FName("Event.Characters.Invoker.Reagent.Quax"));
	
	FAbilityTriggerData ExortTriggerData = FAbilityTriggerData();
	ExortTriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	ExortTriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(FName("Event.Characters.Invoker.Reagent.Exort"));
	
	AbilityTriggers.Add(WexTriggerData);
	AbilityTriggers.Add(QuaxTriggerData);
	AbilityTriggers.Add(ExortTriggerData);
}

void UDTPGAInputReagent::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData * TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	// UAbilityTask_WaitGameplayEvent* WaitGETaskQuax = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,
	// 	FGameplayTag::RequestGameplayTag(FName("Event.Characters.Invoker.Reagent.Quax")));
	// UAbilityTask_WaitGameplayEvent* WaitGETaskWex = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,
	// 	FGameplayTag::RequestGameplayTag(FName("Event.Characters.Invoker.Reagent.Wex")));
	// UAbilityTask_WaitGameplayEvent* WaitGETaskExort = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	// 	this, FGameplayTag::RequestGameplayTag(FName("Event.Characters.Invoker.Reagent.Exort")));
	//
	// WaitGETaskExort->EventReceived.AddDynamic(this, &UDTPGAInputReagent::EventReceived);
	// WaitGETaskQuax->EventReceived.AddDynamic(this, &UDTPGAInputReagent::EventReceived);
	// WaitGETaskWex->EventReceived.AddDynamic(this, &UDTPGAInputReagent::EventReceived);
	//
	// WaitGETaskQuax->ReadyForActivation();
	// WaitGETaskWex->ReadyForActivation();
	// WaitGETaskExort->ReadyForActivation();
	//
	// FGameplayEffectSpecHandle NewHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
	// 	GE_FirstToWex, 1.0, {});
	// auto NewActiveHandle = ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(),
	// 	GetCurrentActivationInfo(), NewHandle);
	EventReceived(*TriggerEventData);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
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
		ShiftReagentQueueII(InvokerReagents::Quax);
	}
	else if (EventData.EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Characters.Invoker.Reagent.Wex")))
	{
		ShiftReagentQueueII(InvokerReagents::Wex);
	}
	else if (EventData.EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Characters.Invoker.Reagent.Exort")))
	{
		ShiftReagentQueueII(InvokerReagents::Exort);
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

	UKismetSystemLibrary::PrintString(this, FString("First reagent value: ") +
		FString::FromInt(static_cast<int>(GetAbilitySystemComponentFromActorInfo()->
		GetNumericAttribute(FirstReagentAttribute))));
	UKismetSystemLibrary::PrintString(this, FString("Second reagent value: ") +
		FString::FromInt(static_cast<int>(GetAbilitySystemComponentFromActorInfo()->
		GetNumericAttribute(SecondReagentAttribute))));
	UKismetSystemLibrary::PrintString(this, FString("Third reagent value: ") +
		FString::FromInt(static_cast<int>(GetAbilitySystemComponentFromActorInfo()->
		GetNumericAttribute(ThirdReagentAttribute))));

}

void UDTPGAInputReagent::ShiftReagentAttributeDuo(float PreviousReagentValue, FGameplayAttribute NextReagentAttribute)
{
	// Create the effect
	UGameplayEffect* GEShiftReagentAttribute = NewObject<UGameplayEffect>(GetTransientPackage(),
		FName(TEXT("ReagentShift")));
	GEShiftReagentAttribute->DurationPolicy = EGameplayEffectDurationType::Instant;
	
	int32 Idx = GEShiftReagentAttribute->Modifiers.Num();
	GEShiftReagentAttribute->Modifiers.SetNum(Idx + 1);
	
	FGameplayModifierInfo& InfoNextReagent = GEShiftReagentAttribute->Modifiers[Idx];
	// InfoNextReagent.Attribute.SetUProperty(NextReagentAttribute.GetUProperty());
	InfoNextReagent.Attribute = NextReagentAttribute;
	InfoNextReagent.ModifierMagnitude = FScalableFloat(PreviousReagentValue);
	InfoNextReagent.ModifierOp = EGameplayModOp::Override;
	
	FGameplayEffectSpec* GESpec = new FGameplayEffectSpec(GEShiftReagentAttribute, {}, 1.0f);
	ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
		FGameplayEffectSpecHandle(GESpec));
}

void UDTPGAInputReagent::ShiftReagentQueueII(int ReagentNumber)
{	// Get attributes
 	FGameplayAttribute FirstReagentAttribute = UDTPAttributeSetInvoker::GetFirstReagentAttribute();
 	FGameplayAttribute SecondReagentAttribute = UDTPAttributeSetInvoker::GetSecondReagentAttribute();
 	FGameplayAttribute ThirdReagentAttribute = UDTPAttributeSetInvoker::GetThirdReagentAttribute();

	// Organize new values for attributes
	float NewFirstReagentValue = static_cast<float>(ReagentNumber);
	float NewSecondReagentValue = GetAbilitySystemComponentFromActorInfo()->
		GetNumericAttribute(FirstReagentAttribute);
	float NewThirdReagentValue = GetAbilitySystemComponentFromActorInfo()->
		GetNumericAttribute(SecondReagentAttribute);

	ApplyEffectBasedOnAttributeAndValue(1, NewFirstReagentValue);
	ApplyEffectBasedOnAttributeAndValue(2, NewSecondReagentValue);
	ApplyEffectBasedOnAttributeAndValue(3, NewThirdReagentValue);
	
	PrintInfo();
}

void UDTPGAInputReagent::PrintInfo_Implementation()
{
 	FGameplayAttribute FirstReagentAttribute = UDTPAttributeSetInvoker::GetFirstReagentAttribute();
 	FGameplayAttribute SecondReagentAttribute = UDTPAttributeSetInvoker::GetSecondReagentAttribute();
 	FGameplayAttribute ThirdReagentAttribute = UDTPAttributeSetInvoker::GetThirdReagentAttribute();
	
	UKismetSystemLibrary::PrintString(this, FString("First reagent value: ") +
		FString::FromInt(static_cast<int>(GetAbilitySystemComponentFromActorInfo()->
		GetNumericAttribute(FirstReagentAttribute))));
	UKismetSystemLibrary::PrintString(this, FString("Second reagent value: ") +
		FString::FromInt(static_cast<int>(GetAbilitySystemComponentFromActorInfo()->
		GetNumericAttribute(SecondReagentAttribute))));
	UKismetSystemLibrary::PrintString(this, FString("Third reagent value: ") +
		FString::FromInt(static_cast<int>(GetAbilitySystemComponentFromActorInfo()->
		GetNumericAttribute(ThirdReagentAttribute))));
}


void UDTPGAInputReagent::ApplyEffectBasedOnAttributeAndValue(int ReagentNumber, float NewValue)
{
	if (ReagentNumber == 1)
	{
		if (NewValue == 0.0f)
		{
			return;
		}
		else if (NewValue == 1.0f)
		{
			FGameplayEffectSpecHandle NewHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
				GE_FirstToQuax, 1.0, {});
			auto Handle = ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(),
				GetCurrentActivationInfo(), NewHandle);
		}
		else if (NewValue == 2.0f)
		{
			FGameplayEffectSpecHandle NewHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
				GE_FirstToWex, 1.0, {});
			auto Handle = ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(),
				GetCurrentActivationInfo(), NewHandle);
		}
		else if (NewValue == 3.0f)
		{
			FGameplayEffectSpecHandle NewHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
				GE_FirstToExort, 1.0, {});
			auto Handle = ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(),
				GetCurrentActivationInfo(), NewHandle);
		}
	}
	else if (ReagentNumber == 2)
	{
		if (NewValue == 0.0f)
		{
			return;
		}
		else if (NewValue == 1.0f)
		{
			FGameplayEffectSpecHandle NewHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
				GE_SecondToQuax, 1.0, {});
			auto Handle = ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
				NewHandle);
		}
		else if (NewValue == 2.0f)
		{
			FGameplayEffectSpecHandle NewHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
				GE_SecondToWex, 1.0, {});
			auto Handle = ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
				NewHandle);
		}
		else if (NewValue == 3.0f)
		{
			FGameplayEffectSpecHandle NewHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
				GE_SecondToExort, 1.0, {});
			auto Handle = ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
				NewHandle);
		}
	}
	else if (ReagentNumber == 3)
	{
		if (NewValue == 0.0f)
		{
			return;
		}
		else if (NewValue == 1.0f)
		{
			FGameplayEffectSpecHandle NewHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
				GE_ThirdToQuax, 1.0, {});
			auto Handle = ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
				NewHandle);
		}
		else if (NewValue == 2.0f)
		{
			FGameplayEffectSpecHandle NewHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
				GE_ThirdToWex, 1.0, {});
			auto Handle = ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
				NewHandle);
		}
		else if (NewValue == 3.0f)
		{
			FGameplayEffectSpecHandle NewHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
				GE_ThirdToExort, 1.0, {});
			auto Handle = ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
				NewHandle);
		}
	}
}

