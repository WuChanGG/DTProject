// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/Abilities/Invoker/DTPGA_InvokerTornado.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UDTPGA_InvokerTornado::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	
}

void UDTPGA_InvokerTornado::CreateTornadoActor(FGameplayAbilityTargetDataHandle DataHandle)
{
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
}
