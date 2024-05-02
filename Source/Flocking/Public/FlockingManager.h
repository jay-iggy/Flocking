// Fill out your copyright notice in the Description page of Pr
#pragma once

#include "CoreMinimal.h"
#include "../Public/Agent.h"
#include "FlockingManager.generated.h"


UCLASS()
class FLOCKING_API UFlockingManager : public UObject {
	public:
		GENERATED_BODY()
		void Init(UWorld *world, UStaticMeshComponent *mesh);
		FVector Rule1(::AAgent* target); //  move towards the center of mass of neighbors
		FVector Rule2(::AAgent* target); // avoid nearby objects
		FVector Rule3(::AAgent* target); // match velocity of nearby boids
		FVector bound_position(::AAgent* target);
		FVector limit_velocity(::AAgent* target);
		FVector tend_to_place(::AAgent* target);
		float disperse_timer = 0;

		void Flock();
		UPROPERTY(EditAnywhere) FVector minBounds = FVector(-10000.0f, -10000.0f, 0.0f);
		UPROPERTY(EditAnywhere) FVector maxBounds = FVector(10000.0f, 10000.0f, 5000.0f);
		UPROPERTY(EditAnywhere) int AGENT_COUNT = 100;
		UPROPERTY(EditAnywhere) float MAX_SPEED = 100.0f;
		UPROPERTY(EditAnywhere) float CONSTRAINT_FORCE = 10.0f;
		UPROPERTY(EditAnywhere) float m1=1,m2=1,m3=1;
		UPROPERTY(EditAnywhere) FVector tendToPosition;
	

	private:
		UWorld *World;
		bool initialized;
		TArray<class AAgent*> Agents;
};