// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/RPGGameplayAbility.h"
#include "RPGThrowWeaponAbility.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API URPGThrowWeaponAbility : public URPGGameplayAbility
{
	GENERATED_BODY()

public:
	URPGThrowWeaponAbility();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	/*
	 * Launch weapon
	 */
	UFUNCTION()
	void LaunchWeapon();
	
	/*
	 * Disappear weapon that we going to throw
	 */
	UFUNCTION()
	void DissapearWeapon();

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* CurrentWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> WeaponThrowClass;
};
