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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFirstSlotAbilityChangedDelegate, FString, SlotClassName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSecondSlotAbilityChangedDelegate, FString, SlotClassName);

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
	FName InputInvokeActionName = FName("InputInvoke");
	FName InputFirstSlotAbilityActionName = FName("InputFirstSlotAbility");
	FName InputSecondSlotAbilityActionName = FName("InputSecondSlotAbility");

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_PassiveWexBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_PassiveExortBonus;

	// This ability is bound to the "D" keyboard key.
	// This ability will be replaced whenever invoke is called, and it will be put in place of SecondSlotAbility
	// If the gameplay ability Invoke is called and the ability that Invoke would grant is already equals to
	// FirstSlotAbility, nothing happens
	TSubclassOf<UGameplayAbility> FirstSlotAbility;
	// This ability is bound to the "F" keyboard key by default
	// This ability will be replaced by the current value of FirstSlotAbility whenever Invoke is called
	// This ability will be sent to the FirstSlotAbility value, when it's the "second slot" and Invoke is called
	// And the SecondSlotAbility is already the ability which Invoke invoked
	// When the previous case happens, FirstSlotAbility and SecondSlotAbility only basically trade places
	TSubclassOf<UGameplayAbility> SecondSlotAbility;

	void InputInvoke();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> GA_Invoke;

	void InputFirstSlotAbility();

	void InputSecondSlotAbility();

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FFirstSlotAbilityChangedDelegate FirstSlotChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FSecondSlotAbilityChangedDelegate SecondSlotChangedDelegate;
};
