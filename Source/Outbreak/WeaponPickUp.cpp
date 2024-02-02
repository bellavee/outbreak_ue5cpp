// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickUp.h"

AWeaponPickUp::AWeaponPickUp()
{
	WeaponComponent = CreateDefaultSubobject<UTP_WeaponComponent>("TP_Weapon");
	SetRootComponent(WeaponComponent);

	PickUpComponent = CreateDefaultSubobject<UTP_PickUpComponent>("TP_PickUp");
	PickUpComponent->SetupAttachment(WeaponComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SkeletalMeshComponent->SetupAttachment(PickUpComponent);

	PointLight = CreateDefaultSubobject<UPointLightComponent>("Point Light");
	PointLight->SetupAttachment(WeaponComponent);
	PointLight->SetAttenuationRadius(15000);
	PointLight->SetCastShadows(false);
	PointLight->SetIntensity(1000);
	PointLight->SetSourceRadius(10);

}
