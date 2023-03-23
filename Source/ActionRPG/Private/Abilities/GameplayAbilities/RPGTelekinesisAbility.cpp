// Copyright Epic Games, Inc. All Rights Reserved.


#include "Abilities/GameplayAbilities/RPGTelekinesisAbility.h"

#include "RPGCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

URPGTelekinesisAbility::URPGTelekinesisAbility()
{
	Radius = 200.f;
	Power = 1000.f;
	SelectedObject = nullptr;
}

void URPGTelekinesisAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SelectObject();
}

void URPGTelekinesisAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if(bWasCancelled)
	{
		Power = 0.f;
	}
	else
	{
		Power = 2000.f;
	}
	ThrowObject();
}

void URPGTelekinesisAbility::SelectObject()
{
	AActor* OwningActor = GetOwningActorFromActorInfo();
	if(!OwningActor)
	{
		return;
	}
	const FHitResult ObjectResult = GetInterativeObjects();
	if(ObjectResult.GetActor())
	{
		SelectedObject = ObjectResult.GetActor();
		OnObjectGrabbedEvent.Broadcast();
		if(UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(SelectedObject->FindComponentByClass<UStaticMeshComponent>()))
		{
			Mesh->SetSimulatePhysics(false);
			if(IsValid(OwningActor->GetComponentsByTag(UActorComponent::StaticClass(), FName("ObjectSocket"))[0]))
			{
				if(USceneComponent* Socket = Cast<USceneComponent>(OwningActor->GetComponentsByTag(UActorComponent::StaticClass(), FName("ObjectSocket"))[0]))
				{
					SelectedObject->AttachToComponent(Socket, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));

					FLatentActionInfo LatentActionInfo;
					LatentActionInfo.CallbackTarget = this;
					UKismetSystemLibrary::MoveComponentTo(SelectedObject->GetRootComponent(), FVector(0), SelectedObject->GetActorRotation(), false, false, 0.5f, false, EMoveComponentAction::Move, LatentActionInfo);
				}
			}
		}
	}
}

void URPGTelekinesisAbility::ThrowObject()
{
	const AActor* OwningActor = GetOwningActorFromActorInfo();
	if(OwningActor)
	{
		if(SelectedObject)
		{
			if(UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(SelectedObject->FindComponentByClass<UStaticMeshComponent>()))
			{
				const ARPGCharacterBase* Character = Cast<ARPGCharacterBase>(OwningActor);
				SelectedObject->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
				Mesh->SetSimulatePhysics(true);
				Mesh->AddImpulse(UKismetMathLibrary::GetForwardVector(Character->GetControlRotation()) * Power, NAME_None, false);
				SelectedObject = nullptr;
			}
		}
	}
}

FHitResult URPGTelekinesisAbility::GetInterativeObjects() const
{
	FHitResult Result;
	const AActor* OwningActor = GetOwningActorFromActorInfo();
	if (OwningActor && GetWorld())
	{
		const FVector VectorStart = OwningActor->GetActorLocation();
		const FVector VectorEnd = (OwningActor->GetActorForwardVector() * 2000) + VectorStart;
		const TArray<AActor*> ActorToIgnore;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Emplace(ECollisionChannel::ECC_WorldDynamic);
		UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), VectorStart, VectorEnd, Radius, 94.f, ObjectTypes, false, ActorToIgnore, EDrawDebugTrace::None, Result, true);
	}
	return Result;
}
