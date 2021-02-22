// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASC/DTPAT_TurnRate.h"
#include "ASC/Abilities/DTPGABase.h"
#include "DTPGA_InvokerTornado.generated.h"

/**
 * 
 */
UCLASS()
class DTP_API UDTPGA_InvokerTornado : public UDTPGABase
{
	GENERATED_BODY()
	
public:
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void CreateTornadoActor(FGameplayTag EventTag, FGameplayAbilityTargetDataHandle DataHandle);

	TSubclassOf<AActor> TornadoProjectileActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceAwayFromAvatarToSpawnTornado = 50.0f;

	UFUNCTION(BlueprintCallable)
	void CreateTurnRateTask(FGameplayAbilityTargetDataHandle DataHandle);

	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UDTPAT_TurnRate* TurnRateTask;
};

