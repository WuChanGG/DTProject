// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "DTPPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DTP_API ADTPPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ADTPPlayerState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAbilitySystemComponent* ASC;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UDTPAttributeSetInvoker* AttributeSet;
};
