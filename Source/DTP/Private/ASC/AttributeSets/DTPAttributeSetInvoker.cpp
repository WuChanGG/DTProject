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
