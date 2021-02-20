// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/AttributeSets/DTPAttributeSetInvoker.h"
#include "Net/UnrealNetwork.h"

UDTPAttributeSetInvoker::UDTPAttributeSetInvoker()
{
	
}

void UDTPAttributeSetInvoker::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDTPAttributeSetInvoker, FirstReagent, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDTPAttributeSetInvoker, SecondReagent, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDTPAttributeSetInvoker, ThirdReagent, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDTPAttributeSetInvoker, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDTPAttributeSetInvoker, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDTPAttributeSetInvoker, QuaxReagentLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDTPAttributeSetInvoker, WexReagentLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDTPAttributeSetInvoker, ExortReagentLevel, COND_None, REPNOTIFY_Always);
}

void UDTPAttributeSetInvoker::OnRep_FirstReagent(const FGameplayAttributeData& OldReagent)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDTPAttributeSetInvoker, FirstReagent, OldReagent);
}

void UDTPAttributeSetInvoker::OnRep_SecondReagent(const FGameplayAttributeData& OldReagent)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDTPAttributeSetInvoker, SecondReagent, OldReagent);
}

void UDTPAttributeSetInvoker::OnRep_ThirdReagent(const FGameplayAttributeData& OldReagent)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDTPAttributeSetInvoker, ThirdReagent, OldReagent);
}

void UDTPAttributeSetInvoker::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDTPAttributeSetInvoker, HealthRegen, OldHealthRegen);
}

void UDTPAttributeSetInvoker::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDTPAttributeSetInvoker, Health, OldHealth);
}

void UDTPAttributeSetInvoker::OnRep_QuaxReagentLevel(const FGameplayAttributeData& OldLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDTPAttributeSetInvoker, Health, QuaxReagentLevel);
}

void UDTPAttributeSetInvoker::OnRep_WexReagentLevel(const FGameplayAttributeData& OldLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDTPAttributeSetInvoker, Health, WexReagentLevel);
}

void UDTPAttributeSetInvoker::OnRep_ExortReagentLevel(const FGameplayAttributeData& OldLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDTPAttributeSetInvoker, Health, ExortReagentLevel);
}
