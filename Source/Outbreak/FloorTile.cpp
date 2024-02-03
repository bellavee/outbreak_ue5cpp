// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTile.h"

#include "NavigationSystem.h"
#include "OutbreakCharacter.h"
#include "Kismet/KismetSystemLibrary.h"


class UNavigationSystemV1;
// Sets default values
AFloorTile::AFloorTile()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(Scene);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>("Attach Point");
	AttachPoint->SetupAttachment(Scene);

	ExitTrigger = CreateDefaultSubobject<UBoxComponent>("Exit Trigger");
	ExitTrigger->SetupAttachment(Scene);

	PickupArea = CreateDefaultSubobject<UBoxComponent>("Pickup Area");
	PickupArea->SetupAttachment(Scene);

	ObstacleArea = CreateDefaultSubobject<UBoxComponent>("Obstacle Area");
	ObstacleArea->SetupAttachment(Scene);

	ZombieArea = CreateDefaultSubobject<UBoxComponent>("Zombie Area");
	ZombieArea->SetupAttachment(Scene);
	
}

FTransform AFloorTile::GetAttachTransform()
{
	return AttachPoint->GetComponentTransform();
}

// Called when the game starts or when spawned
void AFloorTile::BeginPlay()
{
	Super::BeginPlay();
	
	ExitTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFloorTile::OnOverlapBegin);

	for (int i = 0; i < 6; i++)
	{
		SpawnObstacles();
	}

	for (int i = 0; i < 6; i++)
	{
		SpawnPickup();
	}

	for (int i = 0; i < 6; i++)
	{
		SpawnZombies();
	}
	
	NavMeshBoundsVolume = GetWorld()->SpawnActor<ANavMeshBoundsVolume>(ANavMeshBoundsVolume::StaticClass(), GetAttachTransform());
	NavMeshBoundsVolume->SetActorScale3D(FVector(50.0f, 50.f, 20.f));
	NavMeshBoundsVolume->GetRootComponent()->SetMobility(EComponentMobility::Movable);
	NavMeshBoundsVolume->GetRootComponent()->UpdateBounds();

}

FVector AFloorTile::RandomPointInBoundingBox(UBoxComponent* Area)
{
	FVector BoxExtend = Area->GetScaledBoxExtent();
	FVector RandomPosition;
	
	RandomPosition.X = FMath::RandRange(-BoxExtend.X, BoxExtend.X);
	RandomPosition.Y = FMath::RandRange(-BoxExtend.Y, BoxExtend.Y);
	RandomPosition.Z = BoxExtend.Z;

	return RandomPosition;
}

void AFloorTile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AOutbreakCharacter* Character = Cast<AOutbreakCharacter>(OtherActor))
	{
		this->OnTileExited.Broadcast();
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFloorTile::DestroyActor, 0.5f, false);
	}
}

void AFloorTile::SpawnPickup()
{
	FVector Location = RandomPointInBoundingBox(PickupArea) + PickupArea->GetRelativeLocation();
	FRotator Rotation = FRotator::ZeroRotator;
	FVector Scale = FVector(1.0f, 1.0f, 1.0f);
	FTransform RelativeTransform = FTransform(Rotation, Location, Scale);

	if (PickupSpawnChance < FMath::RandRange(0.0f, 1.0f))
	{
		if (PickupClasses.Num() > 0)
		{
			int32 RandomIndex = FMath::RandHelper(PickupClasses.Num());
			
			APickUp* Pickup = GetWorld()->SpawnActor<APickUp>(PickupClasses[RandomIndex].Get(), RelativeTransform);
	
			Pickup->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Pickup"));
			
		}
	}
}

void AFloorTile::SpawnObstacles()
{
	FVector Location = RandomPointInBoundingBox(ObstacleArea) + ObstacleArea->GetRelativeLocation();
	FRotator Rotation = FRotator::ZeroRotator;
	FVector Scale = FVector(1.0f, 1.0f, 1.0f);
	FTransform RelativeTransform = FTransform(Rotation, Location, Scale);
	
	if (ObstacleSpawnChance < FMath::RandRange(0.0f, 1.0f))
	{
		if (ObstacleClasses.Num() > 0)
		{
			int32 RandomIndex = FMath::RandHelper(ObstacleClasses.Num());
			
			AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(ObstacleClasses[RandomIndex].Get(), RelativeTransform);
			Obstacle->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Obstacle"));
			
		}
	}
}

void AFloorTile::SpawnZombies()
{
	// Define maximum number of attempts to find a non-colliding location
	const int32 MaxSpawnAttempts = 10;
	bool bSpawnedSuccessfully = false;

	FVector Location;
	FTransform RelativeTransform;
	FRotator Rotation = FRotator::ZeroRotator;
	FVector Scale = FVector(1.0f, 1.0f, 1.0f);
	
	for (int32 Attempt = 0; Attempt < MaxSpawnAttempts && !bSpawnedSuccessfully; ++Attempt)
	{
		Location = RandomPointInBoundingBox(ZombieArea) + ZombieArea->GetRelativeLocation();
		Location.Z = 150.0f;
		RelativeTransform = FTransform(Rotation, Location, Scale);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
		if (ZombieSpawnChance < FMath::RandRange(0.0f, 1.0f))
		{
			if (ZombieClasses.Num() > 0)
			{
				int32 RandomIndex = FMath::RandHelper(ZombieClasses.Num());

				AZombie* Zombie = GetWorld()->SpawnActor<AZombie>(ZombieClasses[RandomIndex].Get(), RelativeTransform, SpawnParameters);
				if (Zombie)
				{
					Zombie->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Zombie"));
					bSpawnedSuccessfully = true; 
				}
			}
		}
	}

}


void AFloorTile::DestroyActor()
{
	TArray<AActor*> temp;
	GetAttachedActors(temp);
	for (AActor* AttachedActor : temp)
	{
		AttachedActor->Destroy();
	}
	NavMeshBoundsVolume->Destroy();
	Destroy();
}


// Called every frame
void AFloorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

