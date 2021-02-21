// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/DTPAT_TurnRate.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ASC/AttributeSets/DTPAttributeSetInvoker.h"
#include "Kismet/KismetMathLibrary.h"

UDTPAT_TurnRate::UDTPAT_TurnRate()
{
}

void UDTPAT_TurnRate::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (Ability->GetAvatarActorFromActorInfo() == nullptr)
	{
		EndTask();
		return;
	}

	// Get target rotation
	FVector DirectionToRotateTo = DataHitResultLocation - Ability->GetAvatarActorFromActorInfo()->GetActorLocation();
	FVector NormalizedDirectionToRotateTo = DirectionToRotateTo.GetSafeNormal();
	FRotator TargetRotation = UKismetMathLibrary::Conv_VectorToRotator(NormalizedDirectionToRotateTo);

	// Get turn rate
	float TurnRateAttribute = Ability->GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(
		UDTPAttributeSetInvoker::GetTurnRateAttribute());

	float TurnRateInSeconds = 1.0f / TurnRateAttribute;
	
	FRotator NewAvatarRotation = FMath::RInterpTo(Ability->GetAvatarActorFromActorInfo()->GetActorRotation(),
		TargetRotation, DeltaTime, TurnRateInSeconds);

	Ability->GetAvatarActorFromActorInfo()->SetActorRotation(NewAvatarRotation);

	// Get the dot product of the normalized vectors
	FVector AvatarLocationNormalized = Ability->GetAvatarActorFromActorInfo()->GetActorLocation().GetSafeNormal();
	FVector TargetDataLocationNormalized = DataHitResultLocation.GetSafeNormal();
	float DotProduct = FVector::DotProduct(AvatarLocationNormalized, TargetDataLocationNormalized);

	// If we are looking at the hit result location, broadcast OnCompleted
	// In the case of the Invoker Tornado ability, when we are looking toward the hit result location, we spawn the
	// tornado
	if (FMath::IsNearlyEqual(DotProduct, 1.0f, 0.1f) && ShouldBroadcastAbilityTaskDelegates());
	{
		OnCompleted.Broadcast(FGameplayTag(), DataHandle);
		EndTask();
	}
}

UDTPAT_TurnRate* UDTPAT_TurnRate::TurnRateTask(UGameplayAbility* OwningAbility, FName TaskInstanceName,
	FGameplayTagContainer EventTags, FGameplayAbilityTargetDataHandle DataHandle, bool bStopWhenAbilityEnds)
{
	UDTPAT_TurnRate* MyTask = NewAbilityTask<UDTPAT_TurnRate>(OwningAbility, TaskInstanceName);
	MyTask->EventTags = EventTags;
	MyTask->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	MyTask->DataHandle = DataHandle;
	return MyTask;
}

void UDTPAT_TurnRate::Activate()
{
	if (Ability == nullptr)
	{
		EndTask();
		return;
	}

	UAbilitySystemComponent* ASC = Ability->GetAbilitySystemComponentFromActorInfo();

	if (ASC == nullptr)
	{
		EndTask();
		return;
	}

	FHitResult DataHitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(DataHandle, 0);
	if (!DataHitResult.IsValidBlockingHit())
	{
		EndTask();
		return;
	}

	ASC->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(
		this, &UDTPAT_TurnRate::OnGameplayEvent));
	DataHitResultLocation = DataHitResult.Location;
}

void UDTPAT_TurnRate::ExternalCancel()
{
}

void UDTPAT_TurnRate::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData TempData = *Payload;
		TempData.EventTag = EventTag;

		EventReceived.Broadcast(EventTag, TempData);
	}
}
