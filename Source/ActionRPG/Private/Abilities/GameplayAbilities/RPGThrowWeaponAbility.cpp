// Copyright Epic Games, Inc. All Rights Reserved.


#include "Abilities/GameplayAbilities/RPGThrowWeaponAbility.h"

#include "RPGCharacterBase.h"

URPGThrowWeaponAbility::URPGThrowWeaponAbility()
{
}

void URPGThrowWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARPGCharacterBase* CharacterBase = Cast<ARPGCharacterBase>(GetOwningActorFromActorInfo());
	if(!IsValid(CharacterBase))
	{
		return;
	}

	
	if(CharacterBase->GetWeaponInHand())
	{
		CharacterBase->SetWeaponInHand(false);
		LaunchWeapon();
	}
	else
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void URPGThrowWeaponAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URPGThrowWeaponAbility::LaunchWeapon()
{
	ARPGCharacterBase* CharacterBase = Cast<ARPGCharacterBase>(GetOwningActorFromActorInfo());
	if(!IsValid(CharacterBase))
	{
		return;
	}
	
    UWorld* World = GetWorld(); 
	if(World)
	{
		const UCameraComponent* CameraComponent = Cast<UCameraComponent>(CharacterBase->GetComponentsByTag(UCameraComponent::StaticClass(), FName("ThirdPersonCamera"))[0]);
		const FVector HandSocketLocation = CharacterBase->FindComponentByClass<USkeletalMeshComponent>()->GetSocketLocation(FName("hand_r"));
		DissapearWeapon();
		AActor* SpawnedActor = World->SpawnActor<AActor>(WeaponThrowClass, HandSocketLocation, CameraComponent->GetComponentRotation());
		CharacterBase->SetWeaponThrowable(SpawnedActor);
		USkeletalMeshComponent* SpawnedWeaponMesh = SpawnedActor->FindComponentByClass<USkeletalMeshComponent>();
		const USkeletalMeshComponent* WeaponMesh = CurrentWeapon->FindComponentByClass<USkeletalMeshComponent>();
		if(WeaponMesh && SpawnedWeaponMesh)
		{
			SpawnedWeaponMesh->SetSkeletalMesh(WeaponMesh->SkeletalMesh);
		}
	}
}

void URPGThrowWeaponAbility::DissapearWeapon()
{
	const ARPGCharacterBase* CharacterBase = Cast<ARPGCharacterBase>(GetOwningActorFromActorInfo());
	if(!IsValid(CharacterBase))
	{
		return;
	}
	USkeletalMeshComponent* WeaponMesh = CurrentWeapon->FindComponentByClass<USkeletalMeshComponent>();
	if(WeaponMesh)
	{
		WeaponMesh->SetVisibility(false);
	}
}
