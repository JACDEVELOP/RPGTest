// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/RPGGameplayAbility.h"
#include "RPGRecoverWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponReturned);
/**
 * 
 */
UCLASS()
class ACTIONRPG_API URPGRecoverWeapon : public URPGGameplayAbility
{
	GENERATED_BODY()
	
public:
	URPGRecoverWeapon();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	/*
	 * Recover the weapon
	 */
	UFUNCTION()
	void RecoverWeapon();
	
	UFUNCTION()
	void MoveCompleted() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* CurrentWeapon;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnWeaponReturned OnWeaponReturnedEvent;
};
