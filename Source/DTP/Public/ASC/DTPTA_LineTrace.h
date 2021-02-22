// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "DTPTA_LineTrace.generated.h"

/**
 * 
 */
// A simple target actor that will check for the trace channel DTP_Floor (ECC_GameTraceChannel1)
UCLASS()
class DTP_API ADTPTA_LineTrace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
};
