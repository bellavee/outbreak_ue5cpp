// Copyright Epic Games, Inc. All Rights Reserved.

#include "OutbreakGameMode.h"
#include "OutbreakCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


AOutbreakGameMode::AOutbreakGameMode()
{
	InitTileNumber = 2;
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

	AFloorTile* Tile = GetWorld()->SpawnActor<AFloorTile>(TileClass.Get(), NextAttachPoint);
	Tile->OnTileExited.AddDynamic(this, &AOutbreakGameMode::AddFloorTile); 
    
	NextAttachPoint = Tile->GetAttachTransform();

}

void AOutbreakGameMode::RestartLevel()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("RestartLevel");
}

