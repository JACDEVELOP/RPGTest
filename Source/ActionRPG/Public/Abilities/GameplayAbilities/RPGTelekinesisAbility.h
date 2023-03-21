// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/RPGGameplayAbility.h"
#include "RPGTelekinesisAbility.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObjectGrabbed);
/**
 * 
 */
UCLASS()
class ACTIONRPG_API URPGTelekinesisAbility : public URPGGameplayAbility
{
	GENERATED_BODY()

	/************************ Functions ****************************/
public:
	URPGTelekinesisAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	/*
	 * Select object to throw
	 */
	UFUNCTION()
	void SelectObject();
	
	/*
	 * Select object to throw
	 */
	UFUNCTION(BlueprintCallable)
	AActor* GetSelectedObject() const {return SelectedObject;}

	/*
	 * Throw the grabbed object
	 */
	UFUNCTION()
	void ThrowObject();
	
	/*
	 * Get interactuable item to throw
	 */
	UFUNCTION(BlueprintCallable)
	FHitResult GetInterativeObjects() const;
	/************************* Variable ****************************/
	
private:
	UPROPERTY()
	AActor* SelectedObject;

public:
	UPROPERTY(EditAnywhere)
	float Radius;
	
	UPROPERTY(EditAnywhere)
	float Power;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnObjectGrabbed OnObjectGrabbedEvent;
};
