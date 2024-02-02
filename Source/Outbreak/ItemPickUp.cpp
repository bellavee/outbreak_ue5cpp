// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPickUp.h"

AItemPickUp::AItemPickUp()
{
	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	PointLight = CreateDefaultSubobject<UPointLightComponent>("Point Light");
	PointLight->SetupAttachment(Scene);
	PointLight->SetAttenuationRadius(15000);
	PointLight->SetCastShadows(false);
	PointLight->SetIntensity(1000);
	PointLight->SetSourceRadius(10);

	PickUpComponent = CreateDefaultSubobject<UTP_PickUpComponent>("TP_PickUp");
	PickUpComponent->SetupAttachment(Scene);
	
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>("Rotating Movement");
	
}
