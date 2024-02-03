// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle.h"
#include "PickUp.h"
#include "Zombie.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "FloorTile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFloorTile);

UCLASS()
class OUTBREAK_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorTile();

	UFUNCTION(BlueprintCallable)
	FTransform GetAttachTransform();

	UPROPERTY(BlueprintAssignable)
	FFloorTile OnTileExited;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, Category=Tile)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, Category=Tile)
	UArrowComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere, Category=Tile)
	UBoxComponent* ExitTrigger;

	UPROPERTY(VisibleAnywhere, Category=Pickup)
	UBoxComponent* PickupArea;

	UPROPERTY(EditAnywhere, Category=Pickup)
	TArray<TSubclassOf<APickUp>> PickupClasses;
	
	UPROPERTY(EditAnywhere, Category=Pickup)
	float PickupSpawnChance;

	UPROPERTY(VisibleAnywhere, Category=Obstacle)
	UBoxComponent* ObstacleArea;

	UPROPERTY(EditAnywhere, Category=Obstacle)
	TArray<TSubclassOf<AObstacle>> ObstacleClasses;

	UPROPERTY(EditAnywhere, Category=Obstacle)
	float ObstacleSpawnChance;

	UPROPERTY(VisibleAnywhere, Category=Zombie)
	UBoxComponent* ZombieArea;

	UPROPERTY(EditAnywhere, Category=Zombie)
	TArray<TSubclassOf<AZombie>> ZombieClasses;

	UPROPERTY(EditAnywhere, Category=Zombie)
	float ZombieSpawnChance;
	
	UFUNCTION()
	FVector RandomPointInBoundingBox(UBoxComponent* Area);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SpawnPickup();

	UFUNCTION() 
	void SpawnObstacles();
	
	UFUNCTION()
	void SpawnZombies();
	
	UFUNCTION()
	void DestroyActor();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
