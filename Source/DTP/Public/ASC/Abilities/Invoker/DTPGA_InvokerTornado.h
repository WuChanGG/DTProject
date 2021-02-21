// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	void CreateTornadoActor(FGameplayAbilityTargetDataHandle DataHandle);

	TSubclassOf<AActor> TornadoProjectileActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceAwayFromAvatarToSpawnTornado = 50.0f;

};
