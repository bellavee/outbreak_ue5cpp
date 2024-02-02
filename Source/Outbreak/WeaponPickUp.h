// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "TP_PickUpComponent.h"
#include "TP_WeaponComponent.h"
#include "WeaponPickUp.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API AWeaponPickUp : public APickUp
{
	GENERATED_BODY()

public:
	AWeaponPickUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components)
	UTP_WeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, Category=Components)
	UTP_PickUpComponent* PickUpComponent;

	UPROPERTY(VisibleAnywhere, Category=Components)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, Category=Components)
	UPointLightComponent* PointLight;
	
};
