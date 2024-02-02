// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "TP_PickUpComponent.h"
#include "Components/PointLightComponent.h"
#include "ItemPickUp.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API AItemPickUp : public APickUp
{
	GENERATED_BODY()

public:
	AItemPickUp();

	UPROPERTY(VisibleAnywhere, Category=Components)
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, Category=Components)
	UPointLightComponent* PointLight;

	UPROPERTY(VisibleAnywhere, Category=Components)
	UTP_PickUpComponent* PickUpComponent;

	UPROPERTY(VisibleAnywhere, Category=Components)
	URotatingMovementComponent* RotatingMovement;
	
};
