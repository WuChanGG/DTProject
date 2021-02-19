// Fill out your copyright notice in the Description page of Project Settings.


#include "DTP/GameMode/DTPPlayerState.h"
#include "ASC/AttributeSets/DTPAttributeSetInvoker.h"
#include "Kismet/KismetSystemLibrary.h"

ADTPPlayerState::ADTPPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(FName("ASC"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UDTPAttributeSetInvoker>(FName("InvokerAttributes"));

	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* ADTPPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

void ADTPPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
