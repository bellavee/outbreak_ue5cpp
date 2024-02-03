// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FloorTile.h"
#include "GameFramework/GameModeBase.h"
#include "OutbreakGameMode.generated.h"


UCLASS(minimalapi)
class AOutbreakGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOutbreakGameMode();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void AddFloorTile();

	UFUNCTION()
	void RestartLevel();

	UPROPERTY(BlueprintReadOnly)
	FTransform NextAttachPoint;

	UPROPERTY(VisibleAnywhere)
	int32 InitTileNumber;

	UPROPERTY(EditAnywhere)
	TAssetSubclassOf<AFloorTile> TileClass;

};



