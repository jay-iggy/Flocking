#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Agent.generated.h"

UCLASS()
class FLOCKING_API AAgent : public AActor {
	public:
		GENERATED_BODY()
		AAgent();
		void Init(UStaticMeshComponent *mesh, int id);
		UStaticMeshComponent * Mesh;
		FVector Velocity;
		TArray<AActor*> OverlappingActors;
	protected:
		virtual void BeginPlay() override;
	public:
		virtual void Tick(float DeltaTime) override;
};