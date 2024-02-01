// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTile.h"

#include "OutbreakCharacter.h"

// Sets default values
AFloorTile::AFloorTile()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>("Attach Point");
	AttachPoint->SetupAttachment(Scene);

	ExitTrigger = CreateDefaultSubobject<UBoxComponent>("Exit Trigger");
	ExitTrigger->SetupAttachment(Scene);

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

void AFloorTile::DestroyActor()
{
	TArray<AActor*> temp;
	GetAttachedActors(temp);
	for (AActor* AttachedActor : temp)
	{
		AttachedActor->Destroy();
	}
	Destroy();
}

// Called every frame
void AFloorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

