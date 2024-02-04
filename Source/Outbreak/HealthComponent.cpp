// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "OutbreakCharacter.h"
#include "Zombie.h"
#include "Kismet/GameplayStatics.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DefaultHealth = 100;
	Health = DefaultHealth;
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
	
}

float UHealthComponent::AddHealth(float Value)
{
	Health += Value;
	return Health;
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0) return;

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	if (Health <= 0)
	{
		if (AZombie* Zombie = Cast<AZombie>(DamagedActor))
		{
			Zombie->Destroy();
			AOutbreakCharacter* Character = Cast<AOutbreakCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			Character->CountZombie();
		}

		if (AOutbreakCharacter* Character = Cast<AOutbreakCharacter>(DamagedActor))
		{
			Character->Die();
		}
	}
}



