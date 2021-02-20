// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ASC/AttributeSets/DTPAttributeSetInvoker.h"
#include "Abilities/GameplayAbility.h"
#include "CoreMinimal.h"
#include "DTP/DTPCharacter.h"
#include "AbilitySystemInterface.h"
#include "Engine/DataTable.h"
#include "DTP/Public/ASC/AttributeSets/DTPAttributeSetInvoker.h"
#include "DTPCharacterInvoker.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FActiveReagentEffectInfo
{
	GENERATED_BODY()
public:
	
	InvokerReagents Reagent;
	TArray<FActiveGameplayEffectHandle> ActiveEffects;
};

USTRUCT(BlueprintType)
struct FScalableFloatEffectTable : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectModifier;
};

UCLASS()
class DTP_API ADTPCharacterInvoker : public ADTPCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:

	ADTPCharacterInvoker();
	
	void OnRep_PlayerState() override;

	void PossessedBy(AController* NewController) override;

	UAbilitySystemComponent* ASC;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// When the ability system is replicated
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ActivateReagentShiftAbility();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void GiveAbilityListToCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UGameplayAbility>> AbilityList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> ReagentShiftAbility;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	FName InputQuaxActionName = FName("InputQuax");
	FName InputWexActionName = FName("InputWex");
	FName InputExortActionName = FName("InputExort");

	template<InvokerReagents Reagent>
	void InputReagent();

	void InitializeAttributes();

	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> AttributeInitializerGameplayEffect;

	void Tick(float DeltaSeconds) override;

	float TickTime;

	float Cooldown = 5.0f;

	// We want to add the right effect whenever the reagent attribute changes
	// We want to remove the right effects only for the PREVIOUS value of the Third Reagent
	// Because we drop it
	virtual void BindReagentAttributeChangeDelegates();

	// We only care when the first reagent changes, because we are going to replace the value
	// of the second reagent with the relevant values of the first reagent, same for changing
	// the values of the third reagent with the relevant values of the second reagent
	// the values of the third reagent are dropped
	virtual void FirstReagentAttributeChanged(const FOnAttributeChangeData& Data);

	virtual void ReactToReagentAttributeChange(float OldValue, float NewValue);

	virtual void ReagentChangedToQuax();
	virtual void ReagentChangedToWex();
	virtual void ReagentChangedToExort();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_QuaxHealthRegen;

	void DebugMember();

	FActiveGameplayEffectHandle QuaxRegenHandle;

	bool bDebugBool = false;

	FActiveReagentEffectInfo FirstReagentActiveEffectInfo;
	FActiveReagentEffectInfo SecondReagentActiveEffectInfo;
	FActiveReagentEffectInfo ThirdReagentActiveEffectInfo;
	
	void RemoveDroppedThirdReagentEffects(FActiveReagentEffectInfo DroppedReagentInfo);
};
