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

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_HealthRegen)
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UDTPAttributeSetInvoker, HealthRegen);
	
	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UDTPAttributeSetInvoker, Health);
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	UPROPERTY(BlueprintReadOnly, Category = "Reagents", ReplicatedUsing = OnRep_QuaxReagentLevel)
	FGameplayAttributeData QuaxReagentLevel;
	ATTRIBUTE_ACCESSORS(UDTPAttributeSetInvoker, QuaxReagentLevel);
	
	UFUNCTION()
	void OnRep_QuaxReagentLevel(const FGameplayAttributeData& OldLevel);
	
	UPROPERTY(BlueprintReadOnly, Category = "Reagents", ReplicatedUsing = OnRep_WexReagentLevel)
	FGameplayAttributeData WexReagentLevel;
	ATTRIBUTE_ACCESSORS(UDTPAttributeSetInvoker, WexReagentLevel);
	
	UFUNCTION()
	void OnRep_WexReagentLevel(const FGameplayAttributeData& OldLevel);
	
	UPROPERTY(BlueprintReadOnly, Category = "Reagents", ReplicatedUsing = OnRep_ExortReagentLevel)
	FGameplayAttributeData ExortReagentLevel;
	ATTRIBUTE_ACCESSORS(UDTPAttributeSetInvoker, ExortReagentLevel);
	
	UFUNCTION()
	void OnRep_ExortReagentLevel(const FGameplayAttributeData& OldLevel);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_AttackSpeed)
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UDTPAttributeSetInvoker, AttackSpeed);
	
	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UDTPAttributeSetInvoker, MovementSpeed);
	
	UFUNCTION()
	void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_AttackDamage)
	FGameplayAttributeData AttackDamage;
	ATTRIBUTE_ACCESSORS(UDTPAttributeSetInvoker, AttackDamage);
	
	UFUNCTION()
	void OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MinAttackDamage)
	FGameplayAttributeData MinAttackDamage;
	ATTRIBUTE_ACCESSORS(UDTPAttributeSetInvoker, MinAttackDamage);
	
	UFUNCTION()
	void OnRep_MinAttackDamage(const FGameplayAttributeData& OldAttackDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxAttackDamage)
	FGameplayAttributeData MaxAttackDamage;
	ATTRIBUTE_ACCESSORS(UDTPAttributeSetInvoker, MaxAttackDamage);
	
	UFUNCTION()
	void OnRep_MaxAttackDamage(const FGameplayAttributeData& OldAttackDamage);
	
	int ValueOfEmptyReagentSlot = -1;
};
