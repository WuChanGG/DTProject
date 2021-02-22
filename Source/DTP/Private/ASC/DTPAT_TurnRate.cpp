// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/DTPAT_TurnRate.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ASC/AttributeSets/DTPAttributeSetInvoker.h"
#include "Kismet/KismetMathLibrary.h"

UDTPAT_TurnRate::UDTPAT_TurnRate()
{
	bStopWhenAbilityEnds = true;
	bTickingTask = true;
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
	float ConstantTurnRate = TurnRateInSeconds * 60.0f;

	// Calculate time to do a 180 degree turn
	float SecondsToPerformHalfCircleTurn = (0.03f * 3.1416) / TurnRateAttribute;
	// How many degrees are we turning
	float AvatarYaw = Ability->GetAvatarActorFromActorInfo()->GetActorRotation().Yaw;
	float TargetYaw = TargetRotation.Yaw;
	float TurningDegrees = 0.0f;
	

	// FIXED
	// Get degrees between the target rotations
	FVector yy = NormalizedDirectionToRotateTo;
	FVector xx = UKismetMathLibrary::Conv_RotatorToVector(
		Ability->GetAvatarActorFromActorInfo()->GetActorRotation()).GetSafeNormal();
	// xx.X = 0.0f;
	// xx.Y = 0.0f;
	// yy.X = 0.0f;
	// yy.Y = 0.0f;
	// FVector AvatarRV = FVector(0.0f, 0.0f, AvatarYaw);
	// FVector TargetRV = FVector(0.0f, 0.0f, TargetYaw);
	// float Result = FVector::DotProduct(AvatarRV, TargetRV);
	// Result = Result / (AvatarRV.Size() * TargetRV.Size());
	// Result = FMath::Acos(Result);
	float Result = FVector::DotProduct(xx, yy);
	Result = Result / (xx.Size() * yy.Size());
	Result = FMath::Acos(Result);

	// FIXED
	
	if (FMath::Abs(AvatarYaw) > FMath::Abs(TargetYaw))
	{
		TurningDegrees = FMath::Abs(AvatarYaw) - FMath::Abs(TargetYaw);
	}
	else if (FMath::Abs(AvatarYaw) < FMath::Abs(TargetYaw))
	{
		TurningDegrees = FMath::Abs(TargetYaw) - FMath::Abs(AvatarYaw);
	}

	// Three way rule
	//float TimeToPerformTurningDegreesTurn = (TurningDegrees * SecondsToPerformHalfCircleTurn) / 3.1416f;
	float TimeToPerformTurningDegreesTurn = (0.03f * Result) / TurnRateAttribute;
	//float TimeToPerformTurningDegreesTurn = (0.03f * (TurningDegrees * (3.1416f / 180.0f)))  / TurnRateAttribute;

	// Transform time to units per second

	// 0.5 seconds with 1 unit is 2 seconds
	// 0.14 seconds to do 180 units. 0.14 x 60 X 180
	
	FRotator NewAvatarRotation = FMath::RInterpConstantTo(Ability->GetAvatarActorFromActorInfo()->GetActorRotation(),
		TargetRotation, DeltaTime, ((Result * 180.0f) / 3.1416f) / TimeToPerformTurningDegreesTurn );
	FRotator NewAvatarRotationTest = FMath::RInterpTo(Ability->GetAvatarActorFromActorInfo()->GetActorRotation(),
		TargetRotation, DeltaTime, TimeToPerformTurningDegreesTurn * TurningDegrees);
	NewAvatarRotation.Pitch = 0.0f;
	NewAvatarRotation.Roll = 0.0f;
	NewAvatarRotationTest.Pitch = 0.0f;
	NewAvatarRotationTest.Roll = 0.0f;

	FRotator AvatarRotation = Ability->GetAvatarActorFromActorInfo()->GetActorRotation();

    Ability->GetAvatarActorFromActorInfo()->SetActorRotation(NewAvatarRotation);
	// APawn* AvatarPawn = Cast<APawn>(Ability->GetAvatarActorFromActorInfo());
	// AvatarPawn->AddControllerYawInput(NewAvatarRotation.Yaw);

	// Get the dot product of the normalized vectors
	FVector RotationOfAvatarAsVector = UKismetMathLibrary::Conv_RotatorToVector(Ability->GetAvatarActorFromActorInfo()->GetActorRotation()).GetSafeNormal();
	float DotProduct = FVector::DotProduct(RotationOfAvatarAsVector, NormalizedDirectionToRotateTo);
	

	// If we are looking at the hit result location, broadcast OnCompleted
	// In the case of the Invoker Tornado ability, when we are looking toward the hit result location, we spawn the
	// tornado
	if (FMath::IsNearlyEqual(DotProduct, 1.0f, 0.2f) && ShouldBroadcastAbilityTaskDelegates())
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

	EventHandle = ASC->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(
		this, &UDTPAT_TurnRate::OnGameplayEvent));

	CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UDTPAT_TurnRate::OnAbilityCancelled);
	DataHitResultLocation = DataHitResult.Location;
}

void UDTPAT_TurnRate::ExternalCancel()
{
	check(Ability->GetAbilitySystemComponentFromActorInfo())

	OnAbilityCancelled();

	Super::ExternalCancel();

	EndTask();
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

void UDTPAT_TurnRate::OnAbilityCancelled()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		EndTask();
	}
}

void UDTPAT_TurnRate::OnDestroy(bool bInOwnerFinished)
{
	if (Ability != nullptr)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
	}

	if (Ability != nullptr && Ability->GetAbilitySystemComponentFromActorInfo() != nullptr)
	{
		Ability->GetAbilitySystemComponentFromActorInfo()->RemoveGameplayEventTagContainerDelegate(EventTags,
			EventHandle);
	}

	Super::OnDestroy(bInOwnerFinished);

	EndTask();
}

