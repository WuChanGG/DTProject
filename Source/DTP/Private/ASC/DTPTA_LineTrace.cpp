// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/DTPTA_LineTrace.h"

#include "Abilities/GameplayAbility.h"

void ADTPTA_LineTrace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	if (Ability == nullptr)
	{
		CancelTargeting();
		return;
	}

	if (Ability->GetActorInfo().PlayerController == nullptr)
	{
		CancelTargeting();
		return;
	}
}

void ADTPTA_LineTrace::ConfirmTargetingAndContinue()
{
	check(OwningAbility);
	check(OwningAbility->GetActorInfo().PlayerController.Get());
	if (OwningAbility == nullptr)
	{
		CancelTargeting();
		return;
	}

	FHitResult OutHitResult;
	bool bHitSomething = OwningAbility->GetActorInfo().PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), true, OutHitResult);
	if (!bHitSomething || !OutHitResult.IsValidBlockingHit())
	{
		CancelTargeting();
		return;
	}

	FGameplayAbilityTargetData_SingleTargetHit* ReturnData = new FGameplayAbilityTargetData_SingleTargetHit();
	ReturnData->HitResult = OutHitResult;
	FGameplayAbilityTargetDataHandle Handle(ReturnData);
	TargetDataReadyDelegate.Broadcast(Handle);
}
