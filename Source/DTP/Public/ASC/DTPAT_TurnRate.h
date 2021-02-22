// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "DTPAT_TurnRate.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDTPTurnRateDelegate, FGameplayTag, EventTag, FGameplayAbilityTargetDataHandle, DataHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDTPTurnRateEventDelegate, FGameplayTag, EventTag, FGameplayEventData, Payload);

UCLASS()
class DTP_API UDTPAT_TurnRate : public UAbilityTask
{
	GENERATED_BODY()

public:
	UDTPAT_TurnRate();

	virtual void TickTask(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FDTPTurnRateDelegate OnCompleted;

	// This will be used to detect events that would cancel this task and 
	UPROPERTY(BlueprintAssignable)
	FDTPTurnRateEventDelegate EventReceived;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UDTPAT_TurnRate* TurnRateTask(UGameplayAbility* OwningAbility, FName TaskInstanceName,
		FGameplayTagContainer EventTags, FGameplayAbilityTargetDataHandle DataHandle, bool bStopWhenAbilityEnds = true);

	FGameplayTagContainer EventTags;

	bool bStopWhenAbilityEnds;

	FGameplayAbilityTargetDataHandle DataHandle;

	void Activate() override;

	FVector DataHitResultLocation;

	void ExternalCancel() override;

	void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);

	void OnAbilityCancelled();

	FDTPTurnRateEventDelegate OnCancelled;

	FDelegateHandle EventHandle;

	FDelegateHandle CancelledHandle;

	void OnDestroy(bool bInOwnerFinished) override;
};
