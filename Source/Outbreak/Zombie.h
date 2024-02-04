// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

UCLASS()
class OUTBREAK_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Stats)
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Stats)
	float AttackCoolDown;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, Category=Stats)
	UHealthComponent* HealthComponent;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
