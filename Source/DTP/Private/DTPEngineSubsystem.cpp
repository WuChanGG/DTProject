// Fill out your copyright notice in the Description page of Project Settings.


#include "DTPEngineSubsystem.h"
#include "AbilitySystemGlobals.h"

void UDTPEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UAbilitySystemGlobals::Get().InitGlobalData();
}
