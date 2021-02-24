// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/DTPAT_TurnRate.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ASC/AttributeSets/DTPAttributeSetInvoker.h"
#include "Characters/DTPCharacterInvoker.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

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

	// Get degrees between the target rotations
	FVector yy = NormalizedDirectionToRotateTo;
	FVector xx = UKismetMathLibrary::Conv_RotatorToVector(
		Ability->GetAvatarActorFromActorInfo()->GetActorRotation()).GetSafeNormal();
	float Result = FVector::DotProduct(xx, yy);
	Result = Result / (xx.Size() * yy.Size());
	Result = FMath::Acos(Result);

	float TimeToPerformTurningDegreesTurn = (0.03f * Result) / TurnRateAttribute;

	FRotator NewAvatarRotation = FMath::RInterpConstantTo(
		Ability->GetAvatarActorFromActorInfo()->GetActorRotation(), TargetRotation, DeltaTime,
		((Result * 180.0f) / 3.1416f) / TimeToPerformTurningDegreesTurn );
	NewAvatarRotation.Pitch = 0.0f;
	NewAvatarRotation.Roll = 0.0f;

    Ability->GetAvatarActorFromActorInfo()->SetActorRotation(NewAvatarRotation);

	// FIXED
	FVector RotationOfAvatarActorAsVector = UKismetMathLibrary::Conv_RotatorToVector(
		Ability->GetAvatarActorFromActorInfo()->GetActorRotation());
	
	RotationOfAvatarActorAsVector.Z = 0.0f;
	
	FVector DirectionToRotateToAsVector = DirectionToRotateTo;
	
	DirectionToRotateToAsVector.Z = 0.0f;
	
	float NewDotProduct = FVector::DotProduct(RotationOfAvatarActorAsVector.GetSafeNormal(),
		DirectionToRotateToAsVector.GetSafeNormal());

	ADTPCharacterInvoker* InvokerCharacter = Cast<ADTPCharacterInvoker>(Ability->GetAvatarActorFromActorInfo());
	if (Ability->GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority)
	{
		// InvokerCharacter->bIsTurnRotationAlmostEqualOnServer = FMath::IsNearlyEqual(NewDotProduct, 1.0f,
		// 	TurnRateErrorTolerance) && ShouldBroadcastAbilityTaskDelegates();
		InvokerCharacter->bIsTurnRotationAlmostEqualOnServer = FMath::IsNearlyEqual(NewDotProduct, 1.0f,
			TurnRateErrorTolerance) && ShouldBroadcastAbilityTaskDelegates();
		InvokerCharacter->SetTurnRotationEqualServerOnClient(InvokerCharacter->bIsTurnRotationAlmostEqualOnServer);
		
		// This variable (bIsTurnRotationAlmostEqualOnClient) is set by the client,
		// the variable is not replicated, so the version of the client
		// is different from the version of the server
		// InvokerCharacter->bIsTurnRotationAlmostEqualOnClient;
	}
	if (Ability->GetOwningActorFromActorInfo()->GetLocalRole() != ROLE_Authority)
	{
		InvokerCharacter->bIsTurnRotationAlmostEqualOnClient = FMath::IsNearlyEqual(NewDotProduct, 1.0f, TurnRateErrorTolerance) &&
			ShouldBroadcastAbilityTaskDelegates();
		// This variable is set 
		InvokerCharacter->SetTurnRotationEqualClientOnServer(InvokerCharacter->bIsTurnRotationAlmostEqualOnClient);

		// This variable (bIsTurnRotationAlmostEqualOnServer) is set by the server so the variable has different
		// values on the server and on the client
		// InvokerCharacter->bIsTurnRotationAlmostEqualOnServer;
	}
	// If we are looking at the hit result location, broadcast OnCompleted
	// In the case of the Invoker Tornado ability, when we are looking toward the hit result location, we spawn the
	// tornado
	if (InvokerCharacter->bIsTurnRotationAlmostEqualOnClient && InvokerCharacter->bIsTurnRotationAlmostEqualOnServer)
	{

		// Both of the variables 

		
		// if (Ability->GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority)
		// {
		// 	// InvokerCharacter->SetDidTurnRateTaskEndOnServer(true);
		// 	InvokerCharacter->SetTurnRotationEqualClientOnServer(false);
		// }
		// else if (Ability->GetOwningActorFromActorInfo()->GetLocalRole() != ROLE_Authority)
		// {
		// 	InvokerCharacter->SetTurnRotationEqualServerOnServer(false);
		// 	// InvokerCharacter->SetDidTurnRateTaskEndOnClient(true);
		// }

		// if (InvokerCharacter->bDidTurnRateTaskEndOnClient && InvokerCharacter->bDidTurnRateTaskEndOnServer)
		// {
		// 	ClearControlVariables();
		// }
		
		OnCompleted.Broadcast(FGameplayTag(), DataHandle);
		EndTask();
	}
}

void UDTPAT_TurnRate::SetCleanupControlVariables()
{
	if (Ability->GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority)
	{
		SetIsTaskFinishedOnTheServer(true);
	}
	else
	{
		SetIsTaskFinishedOnTheClient(true);
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

	if (Ability->GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority)
	{
		UKismetSystemLibrary::PrintString(this, FString("Task ended on server"));
		UKismetSystemLibrary::PrintString(this, FString("Server rotation: ") + Ability->GetAvatarActorFromActorInfo()->GetActorRotation().ToString());
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, FString("Task ended on client"));
		UKismetSystemLibrary::PrintString(this, FString("Client rotation: ") + Ability->GetAvatarActorFromActorInfo()->GetActorRotation().ToString());
	}
	
	ADTPCharacterInvoker* InvokerCharacter = Cast<ADTPCharacterInvoker>(Ability->GetAvatarActorFromActorInfo());
	if (InvokerCharacter != nullptr)
	{
		InvokerCharacter->bIsTurnRotationAlmostEqualOnClient = false;
		InvokerCharacter->bIsTurnRotationAlmostEqualOnServer = false;
	}
		
	Super::OnDestroy(bInOwnerFinished);

	EndTask();
}

void UDTPAT_TurnRate::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UDTPAT_TurnRate, bIsTurnRotationAlmostEqualOnClient);
	DOREPLIFETIME(UDTPAT_TurnRate, bIsTurnRotationAlmostEqualOnServer);
}

void UDTPAT_TurnRate::OnRep_TaskFinishedServer()
{
	ClearControlVariables();
}

void UDTPAT_TurnRate::OnRep_TaskFinishedClient()
{
	ClearControlVariables();
}

void UDTPAT_TurnRate::SetIsTaskFinishedOnTheClient_Implementation(bool InValue)
{
	IsTaskFinishedOnTheServer = InValue;
}

void UDTPAT_TurnRate::SetIsTaskFinishedOnTheServer_Implementation(bool InValue)
{
	IsTaskFinishedOnTheClient = InValue;
}

void UDTPAT_TurnRate::ClearControlVariables()
{
	ADTPCharacterInvoker* InvokerCharacter = Cast<ADTPCharacterInvoker>(Ability->GetAvatarActorFromActorInfo());
	if (InvokerCharacter->bDidTurnRateTaskEndOnClient && InvokerCharacter->bDidTurnRateTaskEndOnServer)
	{
			InvokerCharacter->SetTurnRotationEqualServerOnClient(false);
			InvokerCharacter->SetTurnRotationEqualClientOnServer(false);
			InvokerCharacter->SetDidTurnRateTaskEndOnServer(false);
			InvokerCharacter->SetDidTurnRateTaskEndOnClient(false);
	}
}

void UDTPAT_TurnRate::ApplyAlmostEqualOnServerTagEffect()
{
	ADTPCharacterInvoker* InvokerCharacter = Cast<ADTPCharacterInvoker>(Ability->GetAvatarActorFromActorInfo());
	if (InvokerCharacter->bDidTurnRateTaskEndOnClient && InvokerCharacter->bDidTurnRateTaskEndOnServer)
	{
		FGameplayEffectSpecHandle SpecHandle = Ability->GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
			InvokerCharacter->GE_TurnRateAlmostEqualOnServer, 1.0,
			Ability->GetAbilitySystemComponentFromActorInfo()->MakeEffectContext());
		Ability->GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UDTPAT_TurnRate::SetFinishedCleanup_Implementation(bool InBool)
{
	FinishedCleanup = InBool;
}

void UDTPAT_TurnRate::SetTurnRotationEqualServerOnServer_Implementation(bool InBool)
{
	bIsTurnRotationAlmostEqualOnServer = InBool;
}

void UDTPAT_TurnRate::SetTurnRotationEqualClientOnServer_Implementation(bool InBool)
{
	bIsTurnRotationAlmostEqualOnClient = InBool;
}
