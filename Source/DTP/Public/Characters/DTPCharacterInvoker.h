// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ASC/AttributeSets/DTPAttributeSetInvoker.h"
#include "Abilities/GameplayAbility.h"
#include "CoreMinimal.h"
#include "DTP/DTPCharacter.h"
#include "AbilitySystemInterface.h"
#include "DTPCharacterInvoker.generated.h"

/**
 * 
 */

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
};
