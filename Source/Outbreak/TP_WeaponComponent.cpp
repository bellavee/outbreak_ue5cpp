// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "OutbreakCharacter.h"
#include "OutbreakProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

UTP_WeaponComponent::UTP_WeaponComponent()
{
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	AmmoAmount = DefaultAmmoAmount;
	bIsLimitAmmo = true;
}

void UTP_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTP_WeaponComponent::SpawnProjectile()
{
	if (ProjectileClass != nullptr && (!bIsLimitAmmo || AmmoAmount > 0) )
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			World->SpawnActor<AOutbreakProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}

		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
		}
	
		if (FireAnimation != nullptr)
		{
			UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}


void UTP_WeaponComponent::FireRifle()
{
	SpawnProjectile();
	if (AmmoAmount > 0)
		AmmoAmount -= 1;
}


void UTP_WeaponComponent::FirePistol()
{
	SpawnProjectile();
	if (AmmoAmount > 0)
		AmmoAmount -= 1;
}


void UTP_WeaponComponent::FireShotgun()
{
	SpawnProjectile();
	if (AmmoAmount % 3 == 0 && AmmoAmount > 0)
		AmmoAmount -= 3;
}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if (bIsRifle) FireRifle();
	if (bIsPistol) FirePistol();
	if (bIsShotgun) FireShotgun();
	
}

void UTP_WeaponComponent::AttachWeapon(AOutbreakCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	if (Character == nullptr)
	{
		return;
	}

	if (Character->GetHasRifle())
	{
		TArray<AActor*> AttachedActors;
		Character->GetAttachedActors(AttachedActors);
		for (AActor* Actor : AttachedActors)
		{
			if (Actor)
			{
				Actor->Destroy();
			}
		}
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	
	Character->SetHasRifle(true);

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
		}
	}
}


void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}
