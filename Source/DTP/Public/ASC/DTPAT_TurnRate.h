// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "DTPAT_TurnRate.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDTPTurnRateDelegate, FGameplayTag, EventTag,
	FGameplayAbilityTargetDataHandle, DataHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDTPTurnRateEventDelegate, FGameplayTag, EventTag,
	FGameplayEventData, Payload);

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

	virtual void Activate() override;

	FVector DataHitResultLocation;

	virtual void ExternalCancel() override;

	void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);

	void OnAbilityCancelled();

	FDTPTurnRateEventDelegate OnCancelled;

	FDelegateHandle EventHandle;

	FDelegateHandle CancelledHandle;

	virtual void OnDestroy(bool bInOwnerFinished) override;

	float TurnRateErrorTolerance = 0.001f;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = "OnRep_TaskFinishedServer")
	bool IsTaskFinishedOnTheServer;

	UFUNCTION()
	void OnRep_TaskFinishedServer();

	UPROPERTY(ReplicatedUsing = "OnRep_TaskFinishedClient")
	bool IsTaskFinishedOnTheClient;

	UFUNCTION()
	void OnRep_TaskFinishedClient();

	UFUNCTION(Server, Reliable)
	void SetIsTaskFinishedOnTheServer(bool InValue);

	UFUNCTION(Server, Reliable)
	void SetIsTaskFinishedOnTheClient(bool InValue);

	void SetCleanupControlVariables();

	void ClearControlVariables();

	UPROPERTY(Replicated)
	bool FinishedCleanup = false;

	UFUNCTION(Server, Reliable)
	void SetFinishedCleanup(bool InBool);
	
	UPROPERTY(Replicated)
	bool bIsTurnRotationAlmostEqualOnServer = false;

	UPROPERTY(Replicated)
	bool bIsTurnRotationAlmostEqualOnClient = false;

	UFUNCTION(Server, Reliable)
	void SetTurnRotationEqualClientOnServer(bool InBool);
	
	UFUNCTION(Server, Reliable)
	void SetTurnRotationEqualServerOnServer(bool InBool);

	void ApplyAlmostEqualOnServerTagEffect();
};
