// Copyright Epic Games, Inc. All Rights Reserved.


#include "Abilities/GameplayAbilities/RPGRecoverWeapon.h"

#include "RPGCharacterBase.h"
#include "Components/TimelineComponent.h"

URPGRecoverWeapon::URPGRecoverWeapon()
{
}

void URPGRecoverWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	ARPGCharacterBase* CharacterBase = Cast<ARPGCharacterBase>(GetOwningActorFromActorInfo());
	if(!IsValid(CharacterBase))
	{
		return;
	}
	
	if(!CharacterBase->GetWeaponInHand())
	{
		CharacterBase->SetWeaponInHand(true);
		RecoverWeapon();
	}
	else
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void URPGRecoverWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URPGRecoverWeapon::RecoverWeapon()
{
	const ARPGCharacterBase* CharacterBase = Cast<ARPGCharacterBase>(GetOwningActorFromActorInfo());
	if(!IsValid(CharacterBase))
	{
		return;
	}
	
	const FVector HandSocketLocation = CharacterBase->FindComponentByClass<USkeletalMeshComponent>()->GetSocketLocation(FName("hand_r"));
	const FRotator HandSocketRotator = CharacterBase->FindComponentByClass<USkeletalMeshComponent>()->GetSocketRotation(FName("hand_r"));
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("MoveCompleted");
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = FMath::Rand();
	UKismetSystemLibrary::MoveComponentTo(CharacterBase->GetWeaponThrowable()->GetRootComponent(),HandSocketLocation, HandSocketRotator,false, true, 0.4, true, EMoveComponentAction::Move, LatentInfo);
}

void URPGRecoverWeapon::MoveCompleted() const
{
	const ARPGCharacterBase* CharacterBase = Cast<ARPGCharacterBase>(GetOwningActorFromActorInfo());
	if(!IsValid(CharacterBase))
	{
		return;
	}
	USkeletalMeshComponent* WeaponMesh = CurrentWeapon->FindComponentByClass<USkeletalMeshComponent>();
	if(WeaponMesh)
	{
		WeaponMesh->SetVisibility(true);
	}
	CharacterBase->GetWeaponThrowable()->Destroy();
	OnWeaponReturnedEvent.Broadcast();
}
