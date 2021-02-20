// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASC/Abilities/DTPGABase.h"
#include "DTPGAInputReagent.generated.h"

// This ability will be activated when the ASC is setup on Invoker, it will listen for gameplay
// events that contain the tag of the new reagent in the reagent queue and shift them accordingly
// This ability will begin at the beginning of the game and only end when the game ends. The only situation
// When we don't listen to events to shift the reagent queue is when the character is dead.
UCLASS()
class DTP_API UDTPGAInputReagent : public UDTPGABase
{
	GENERATED_BODY()
	
public:

	UDTPGAInputReagent();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	virtual void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
    void EventReceived(FGameplayEventData EventData);

	// The attribute FirstReagent will be shifted to SecondReagent, and SecondReagent will be shifted to
	// ThirdReagent and ThirdReagent will be dropped
	virtual void ShiftActiveReagentQueue(int ReagentNumber);

	// E.G. FirstReagent value is shifted to SecondReagent value
	void ShiftReagentAttributeDuo(float PreviousReagentValue, FGameplayAttribute NextReagentAttribute);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_OverrideReagent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, TSubclassOf<UGameplayEffect>> GE_ReagentOverrideList;

	void ShiftReagentQueueII(int ReagentNumber);

	void ApplyEffectBasedOnAttributeAndValue(int ReagentNumber, float NewValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_FirstToWex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_FirstToQuax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_FirstToExort;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_SecondToWex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_SecondToQuax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_SecondToExort;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_ThirdToWex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_ThirdToQuax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_ThirdToExort;

	UFUNCTION(Server, Reliable)
	void PrintInfo();
};

