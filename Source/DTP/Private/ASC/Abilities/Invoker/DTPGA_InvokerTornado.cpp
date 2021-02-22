// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/Abilities/Invoker/DTPGA_InvokerTornado.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UDTPGA_InvokerTornado::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UDTPGA_InvokerTornado::CreateTornadoActor(FGameplayTag EventTag, FGameplayAbilityTargetDataHandle DataHandle)
{
	if (GetOwningActorFromActorInfo()->GetLocalRole() != ROLE_Authority)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
			true, false);
		return;
	}
	
	check(TornadoProjectileActorClass)

	// Get target data
	FHitResult DataHitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(DataHandle, 0);
	if (!DataHitResult.IsValidBlockingHit())
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid hit result @UDTPGA_InvokerTornado::CreateTornadoActor"));
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
			true, true);
		return;
	}

	if (GetAvatarActorFromActorInfo() == nullptr)
	{
		return;
	}

	FVector AvatarLocation = GetAvatarActorFromActorInfo()->GetActorLocation();

	// Get projectile direction relative to avatar actor
	FVector RelativeProjectileDirection = DataHitResult.Location - AvatarLocation;
	FVector NormalizedDirection = RelativeProjectileDirection.GetSafeNormal();

	FVector ProjectileSpawnLocation = AvatarLocation + (NormalizedDirection * DistanceAwayFromAvatarToSpawnTornado);
	FRotator ProjectileSpawnRotation = UKismetMathLibrary::Conv_VectorToRotator(NormalizedDirection);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetAvatarActorFromActorInfo();
	SpawnParameters.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	GetWorld()->SpawnActor<AActor>(TornadoProjectileActorClass, ProjectileSpawnLocation, ProjectileSpawnRotation,
		SpawnParameters);
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
		true, false);
}

void UDTPGA_InvokerTornado::CreateTurnRateTask(FGameplayAbilityTargetDataHandle DataHandle)
{
	// In order to realize any kind of ability we must first realize the TurnRate task successfully
	TurnRateTask = UDTPAT_TurnRate::TurnRateTask(this, FName("TurnRateTask"),
		FGameplayTagContainer(), DataHandle);

	TurnRateTask->OnCompleted.AddDynamic(this, &UDTPGA_InvokerTornado::CreateTornadoActor);
	TurnRateTask->EventReceived.AddDynamic(this, &UDTPGA_InvokerTornado::EventReceived);
	TurnRateTask->OnCancelled.AddDynamic(this, &UDTPGA_InvokerTornado::OnCancelled);

	TurnRateTask->ReadyForActivation();
}

void UDTPGA_InvokerTornado::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Ability.Cancel")))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
			true, true);
	}
}

void UDTPGA_InvokerTornado::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
		true, true);
}

void UDTPGA_InvokerTornado::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	TurnRateTask->EndTask();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
