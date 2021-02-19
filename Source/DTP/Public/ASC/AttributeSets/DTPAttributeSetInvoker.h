// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DTPAttributeSetInvoker.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
enum InvokerReagents : uint8
{
	None,
	// Number 1
	Quax,
	// Number 2
	Wex,
	// Number 3
	Exort
};

UCLASS()
class DTP_API UDTPAttributeSetInvoker : public UAttributeSet
{
	GENERATED_BODY()

public:

	UDTPAttributeSetInvoker();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Reagents", ReplicatedUsing = OnRep_FirstReagent)
	FGameplayAttributeData FirstReagent;
	ATTRIBUTE_ACCESSORS(UDTPAttributeSetInvoker, FirstReagent);

	UFUNCTION()
	void OnRep_FirstReagent(const FGameplayAttributeData& OldReagent);
	
	UPROPERTY(BlueprintReadOnly, Category = "Reagents", ReplicatedUsing = OnRep_SecondReagent)
	FGameplayAttributeData SecondReagent;
	ATTRIBUTE_ACCESSORS(UDTPAttributeSetInvoker, SecondReagent);

	UFUNCTION()
	void OnRep_SecondReagent(const FGameplayAttributeData& OldReagent);

	UPROPERTY(BlueprintReadOnly, Category = "Reagents", ReplicatedUsing = OnRep_ThirdReagent)
	FGameplayAttributeData ThirdReagent;
	ATTRIBUTE_ACCESSORS(UDTPAttributeSetInvoker, ThirdReagent);

	UFUNCTION()
	void OnRep_ThirdReagent(const FGameplayAttributeData& OldReagent);

	int ValueOfEmptyReagentSlot = -1;
};
