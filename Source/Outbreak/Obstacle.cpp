// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	ActionTrigger = CreateDefaultSubobject<UBoxComponent>("Action Trigger");
	ActionTrigger->SetupAttachment(Scene);
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	ActionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OnGet);
	ActionTrigger->OnComponentEndOverlap.AddDynamic(this, &AObstacle::OnOut);
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

