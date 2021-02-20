// Fill out your copyright notice in the Description page of Project Settings.


#include "DTP/Public/Characters/DTPCharacterInvoker.h"
#include "DTP/GameMode/DTPPlayerState.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ASC/Abilities/DTPGAInputReagent.h"
#include "Serialization/StructuredArchiveFormatter.h"
#include "Kismet/KismetSystemLibrary.h"

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
		DebugMember();
		
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
		// ASC->TriggerAbilityFromGameplayEvent(FGameplayAbilitySpec(ReagentShiftAbility).Handle,
		// 	ASC->AbilityActorInfo.Get(), FGameplayTag::RequestGameplayTag(
		// 	FName("Event.Characters.Invoker.Reagent.Quax")), {}, *ASC);
	}
	else if (Reagent == InvokerReagents::Wex)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FGameplayTag::RequestGameplayTag(
			FName("Event.Characters.Invoker.Reagent.Wex")), FGameplayEventData());
		// ASC->TriggerAbilityFromGameplayEvent(FGameplayAbilitySpec(ReagentShiftAbility).Handle,
		// 	ASC->AbilityActorInfo.Get(), FGameplayTag::RequestGameplayTag(
		// 	FName("Event.Characters.Invoker.Reagent.Wex")), {}, *ASC);
	}
	else if (Reagent == InvokerReagents::Exort)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FGameplayTag::RequestGameplayTag(
			FName("Event.Characters.Invoker.Reagent.Exort")), FGameplayEventData());
		// ASC->TriggerAbilityFromGameplayEvent(FGameplayAbilitySpec(ReagentShiftAbility).Handle,
		// 	ASC->AbilityActorInfo.Get(), FGameplayTag::RequestGameplayTag(
		// 	FName("Event.Characters.Invoker.Reagent.Exort")), {}, *ASC);
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

void ADTPCharacterInvoker::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	
	// if (GetWorld()->GetTimeSeconds() - TickTime > Cooldown)
	// {
	// 	FString First = FString::FromInt(ASC->GetNumericAttribute(
	// 		UDTPAttributeSetInvoker::GetFirstReagentAttribute()));
	// 	FString Second = FString::FromInt(ASC->GetNumericAttribute(
	// 		UDTPAttributeSetInvoker::GetSecondReagentAttribute()));
	// 	FString Third = FString::FromInt(ASC->GetNumericAttribute(
	// 		UDTPAttributeSetInvoker::GetThirdReagentAttribute()));
	//
	// 	UKismetSystemLibrary::PrintString(this, FString("Frist Attribute: ") + First);
	// 	UKismetSystemLibrary::PrintString(this, FString("Second Attribute: ") + Second);
	// 	UKismetSystemLibrary::PrintString(this, FString("Third Attribute: ") + Third);
	// 	TickTime = GetWorld()->GetTimeSeconds();
	// }

	if (GetWorld()->GetTimeSeconds() > 20.0f && !bDebugBool)
	{
		ASC->RemoveActiveGameplayEffect(QuaxRegenHandle, 1);
		UKismetSystemLibrary::PrintString(this, FString("Revomed quax effect"));
		bDebugBool = true;
	}
}

void ADTPCharacterInvoker::BindReagentAttributeChangeDelegates()
{
	if (ASC == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ASC @ADTPCharacterInvoker::BindReagentAttributeChangeDelegates"));
		return;
	}

	ASC->GetGameplayAttributeValueChangeDelegate(UDTPAttributeSetInvoker::GetFirstReagentAttribute()).AddUObject(this,
		&ADTPCharacterInvoker::FirstReagentAttributeChanged);
}

void ADTPCharacterInvoker::FirstReagentAttributeChanged(const FOnAttributeChangeData& Data)
{
	// Make temp structures of ActiveReagentEffectInfo
	FActiveReagentEffectInfo TempFirstReagentInfo = FirstReagentActiveEffectInfo;
	FActiveReagentEffectInfo TempSecondReagentInfo = SecondReagentActiveEffectInfo;
	FActiveReagentEffectInfo TempThirdReagentInfo = ThirdReagentActiveEffectInfo;
	
	if (Data.NewValue == static_cast<float>(InvokerReagents::Quax))
	{
		ReagentChangedToQuax();
	}
	else if (Data.NewValue == static_cast<float>(InvokerReagents::Wex))
	{
		ReagentChangedToWex();
	}
	else if (Data.NewValue == static_cast<float>(InvokerReagents::Exort))
	{
		ReagentChangedToExort();
	}

	// After the first reagent has been changed appropriately, we don't need to remove any current effect
	// except effects that were currently in the ThirdReagentInfo

	// First replace the data
	SecondReagentActiveEffectInfo = TempFirstReagentInfo;
	ThirdReagentActiveEffectInfo = TempSecondReagentInfo;

	// Now remove all of the effects caused by the DROPPED third reagent
	RemoveDroppedThirdReagentEffects(TempThirdReagentInfo);
	
}

void ADTPCharacterInvoker::ReactToReagentAttributeChange(float OldValue, float NewValue)
{
	
}

void ADTPCharacterInvoker::DebugMember()
{
	check(GE_QuaxHealthRegen);
	check(ASC);
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GE_QuaxHealthRegen, 1.0,
		ASC->MakeEffectContext());
	QuaxRegenHandle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void ADTPCharacterInvoker::RemoveDroppedThirdReagentEffects(FActiveReagentEffectInfo DroppedReagentInfo)
{
	if (DroppedReagentInfo.Reagent == InvokerReagents::None || DroppedReagentInfo.ActiveEffects.Num() <= 0)
	{
		return;
	}

	for (FActiveGameplayEffectHandle SingleHandle : DroppedReagentInfo.ActiveEffects)
	{
		ASC->RemoveActiveGameplayEffect(SingleHandle, 1);
	}
}

void ADTPCharacterInvoker::ReagentChangedToQuax()
{
	check(GE_QuaxHealthRegen);
	
	// Get current level of QuaxReagent
	float ReagentLevel = ASC->GetNumericAttribute(UDTPAttributeSetInvoker::GetQuaxReagentLevelAttribute());

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	
	FGameplayEffectSpecHandle RegenSpecHandle = ASC->MakeOutgoingSpec(GE_QuaxHealthRegen, ReagentLevel, Context);

	// Add the passive regen effect
	FActiveGameplayEffectHandle ReagentActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*RegenSpecHandle.Data.Get());

	FirstReagentActiveEffectInfo.Reagent = InvokerReagents::Quax;
	FirstReagentActiveEffectInfo.ActiveEffects.Empty();
	FirstReagentActiveEffectInfo.ActiveEffects.Add(ReagentActiveHandle);
}

void ADTPCharacterInvoker::ReagentChangedToWex()
{
}

void ADTPCharacterInvoker::ReagentChangedToExort()
{
}
