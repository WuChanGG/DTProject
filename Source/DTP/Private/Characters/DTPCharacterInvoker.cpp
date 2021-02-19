// Fill out your copyright notice in the Description page of Project Settings.


#include "DTP/Public/Characters/DTPCharacterInvoker.h"
#include "DTP/GameMode/DTPPlayerState.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ASC/Abilities/DTPGAInputReagent.h"
#include "Serialization/StructuredArchiveFormatter.h"

ADTPCharacterInvoker::ADTPCharacterInvoker()
{
	
}

void ADTPCharacterInvoker::BeginPlay()
{
	Super::BeginPlay();
	if (true)
	{
		return;
	}
		
}

void ADTPCharacterInvoker::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ADTPPlayerState* PS = GetPlayerState<ADTPPlayerState>();
	if (PS != nullptr)
	{
		ASC = PS->GetAbilitySystemComponent();

		ASC->InitAbilityActorInfo(PS, this);
		ActivateReagentShiftAbility();
	}
}

// Server only
void ADTPCharacterInvoker::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ADTPPlayerState* PS = GetPlayerState<ADTPPlayerState>();
	if (PS != nullptr)
	{
		ASC = PS->GetAbilitySystemComponent();

		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		InitializeAttributes();
		GiveAbilityListToCharacter();
		
	}

}

UAbilitySystemComponent* ADTPCharacterInvoker::GetAbilitySystemComponent() const
{
	return ASC;
}

void ADTPCharacterInvoker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction(InputExortActionName, EInputEvent::IE_Released, this,
		&ADTPCharacterInvoker::InputReagent<InvokerReagents::Exort>);
	
	PlayerInputComponent->BindAction(InputWexActionName, EInputEvent::IE_Released, this,
		&ADTPCharacterInvoker::InputReagent<InvokerReagents::Wex>);
	
	PlayerInputComponent->BindAction(InputQuaxActionName, EInputEvent::IE_Released, this,
		&ADTPCharacterInvoker::InputReagent<InvokerReagents::Quax>);
}

void ADTPCharacterInvoker::GiveAbilityListToCharacter_Implementation()
{
	if (ASC == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ASC @GiveAbilityListToCharacter_Implementation"));
		return;
	}
	
	for (TSubclassOf<UGameplayAbility> SingleAbility : AbilityList)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(SingleAbility));
	}
}

void ADTPCharacterInvoker::ActivateReagentShiftAbility_Implementation()
{
	if (ASC == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ASC @ADTPCharacterInvoker::ActivateReagentShiftAbility_Implementation"));
		return;
	}
	
	ASC->TryActivateAbilityByClass(ReagentShiftAbility);
}

template <InvokerReagents Reagent>
void ADTPCharacterInvoker::InputReagent()
{
	if (Reagent == InvokerReagents::Quax)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FGameplayTag::RequestGameplayTag(
			FName("Event.Characters.Invoker.Reagent.Quax")), FGameplayEventData());
	}
	else if (Reagent == InvokerReagents::Wex)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FGameplayTag::RequestGameplayTag(
			FName("Event.Characters.Invoker.Reagent.Wex")), FGameplayEventData());
	}
	else if (Reagent == InvokerReagents::Exort)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FGameplayTag::RequestGameplayTag(
			FName("Event.Characters.Invoker.Reagent.Exort")), FGameplayEventData());
	}
}

void ADTPCharacterInvoker::InitializeAttributes()
{
	if (AttributeInitializerGameplayEffect == nullptr || ASC == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No attribute initializer or ASC @ADTPCharacterInvoker::InitializeAttributes()"));
		return;
	}

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	
	FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(AttributeInitializerGameplayEffect, 1.0f,
		ContextHandle);
	
	if (NewHandle.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), ASC);
	}
}