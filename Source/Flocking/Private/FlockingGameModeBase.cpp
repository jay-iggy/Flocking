// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/FlockingGameModeBase.h"

AFlockingGameModeBase::AFlockingGameModeBase() {
	AgentMesh = CreateDefaultSubobject<UStaticMeshComponent>("AgentMesh");
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	AgentMesh->SetStaticMesh(SphereMeshAsset.Object);
}

void AFlockingGameModeBase::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("GAMEMODE BEGINPLAY()"));
	Manager = NewObject<UFlockingManager>();
	Manager->Init( GetWorld(), AgentMesh );
}

void AFlockingGameModeBase::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
	Manager->Flock();
};