// Copyright Epic Games, Inc. All Rights Reserved.

#include "OutbreakGameMode.h"
#include "OutbreakCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


AOutbreakGameMode::AOutbreakGameMode()
{
	InitTileNumber = 3;
}

void AOutbreakGameMode::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < InitTileNumber; i++)
	{
		AddFloorTile();
	}
	
	if (AOutbreakCharacter* Character = Cast<AOutbreakCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		Character->OnDeath.AddDynamic(this, &AOutbreakGameMode::RestartLevel);
	}
}

void AOutbreakGameMode::AddFloorTile()
{
	if (!TileClass) return;
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.bNoFail = true;
	SpawnParameters.Owner = this;
	
	AFloorTile* Tile = GetWorld()->SpawnActor<AFloorTile>(TileClass.Get(), NextAttachPoint, SpawnParameters);
	Tile->OnTileExited.AddDynamic(this, &AOutbreakGameMode::AddFloorTile);
	
	NextAttachPoint = Tile->GetAttachTransform();
}

void AOutbreakGameMode::RestartLevel()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("RestartLevel");
}
