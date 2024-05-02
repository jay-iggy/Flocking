#include "../Public/FlockingManager.h"
#include "../Public/Agent.h"



void UFlockingManager::Init( UWorld *world, UStaticMeshComponent *mesh ) {
	UE_LOG(LogTemp, Warning, TEXT("MANAGER INIT"));

	World = world;

	for( int i = 0; i < AGENT_COUNT; i++ ) {
		if( World != nullptr ) {
			FRotator rotation = FRotator();
			FVector location = FVector();

			// randomize location
			location.X = FMath::RandRange( minBounds.X, maxBounds.X );
			location.Y = FMath::RandRange( minBounds.Y, maxBounds.Y );
			location.Z = FMath::RandRange( minBounds.Z, maxBounds.Z );
			

			AAgent * agent = World->SpawnActor<AAgent>( location, rotation );
			agent->Init( mesh, i );
			Agents.Add( agent );
		}
	}
	// set tendToPosition to a random spot in the bounds
	tendToPosition.X = FMath::RandRange(minBounds.X, maxBounds.X);
	tendToPosition.Y = FMath::RandRange(minBounds.Y, maxBounds.Y);
	tendToPosition.Z = FMath::RandRange(minBounds.Z, maxBounds.Z);

	initialized = true;
}

FVector UFlockingManager::Rule1(AAgent* target){
	FVector perceived_center = FVector();
	for( AAgent *other : Agents ) {
		if( other != target ) {
			perceived_center += other->GetActorLocation();
		}
	}
	perceived_center /= AGENT_COUNT - 1;
	return (perceived_center - target->GetActorLocation()) / 100.f;
}

FVector UFlockingManager::Rule2(AAgent* target){
	FVector displacement = FVector(0,0,0);
	for(AAgent *other : Agents){
		if(other != target){
			if(FVector::Dist(other->GetActorLocation(), target->GetActorLocation()) < 100.f){
				displacement -= (other->GetActorLocation() - target->GetActorLocation());
			}
		}
	}
	for (AActor *actor : target->OverlappingActors) {
		displacement -= (actor->GetActorLocation() - target->GetActorLocation());
	}
	return displacement;
}

FVector UFlockingManager::Rule3(AAgent* target){
	FVector perceived_velocity = FVector();
	for(AAgent *other : Agents){
		if(other != target){
			perceived_velocity += other->Velocity;
		}
	}
	perceived_velocity /= AGENT_COUNT - 1;
	return (perceived_velocity - target->Velocity) / 8.f;
}

FVector UFlockingManager::bound_position(AAgent* target){
	FVector v = target->Velocity;
	FVector loc = target->GetActorLocation();
	if(loc.X < minBounds.X){
		v.X = CONSTRAINT_FORCE;
	}
	if(loc.X > maxBounds.X){
		v.X = -CONSTRAINT_FORCE;
	}
	if(loc.Y < minBounds.Y){
		v.Y = CONSTRAINT_FORCE;
	}
	if(loc.Y > maxBounds.Y){
		v.Y = -CONSTRAINT_FORCE;
	}
	if(loc.Z < minBounds.Z){
		v.Z = CONSTRAINT_FORCE;
	}
	if(loc.Z > maxBounds.Z){
		v.Z = -CONSTRAINT_FORCE;
	}
	return v;
}

FVector UFlockingManager::limit_velocity(AAgent* target){
	FVector v = target->Velocity;
	if(v.Size() > MAX_SPEED){
		v = v.GetClampedToMaxSize(MAX_SPEED);
	}
	return v;
}

FVector UFlockingManager::tend_to_place(AAgent* target) {
	return (tendToPosition - target->GetActorLocation()) / 100.f;
}

void UFlockingManager::Flock() {
	int agentsToLeadFlock = AGENT_COUNT / 3; // how many agents should lead the flock
	
	for( AAgent *agent : Agents ) {
		FVector v = agent->Velocity;
		v += m1 * Rule1( agent ) + m2 * Rule2( agent ) + m3 * Rule3( agent ); // apply rules

		// if agent is one of the agents that should lead the flock, move toward the new tendToPosition
		if(agentsToLeadFlock > 0){
			v += tend_to_place(agent);
			agentsToLeadFlock--;
		}
		
		agent->Velocity = v;
		v = limit_velocity( agent );
		agent->Velocity = v;
		v = bound_position( agent );
		agent->Velocity = v;
	}

	// calculate center of flock
	FVector center = FVector();
	for( AAgent *agent : Agents ) {
		center += agent->GetActorLocation();
	}
	center /= AGENT_COUNT;

	//if flock has reached tendToPosition, move tendToPosition to a random spot in the bounds
	if(FVector::Dist(center, tendToPosition) < 1000.f){
		tendToPosition.X = FMath::RandRange(minBounds.X, maxBounds.X);
		tendToPosition.Y = FMath::RandRange(minBounds.Y, maxBounds.Y);
		tendToPosition.Z = FMath::RandRange(minBounds.Z, maxBounds.Z);

		// shuffle agents so that it isnt the same ones moving to the new tendToPosition and looks more natural
		for(int j = 0; j < AGENT_COUNT / 5; j++){
			int rand = FMath::RandRange(0, AGENT_COUNT - 1);
			int rand2 = FMath::RandRange(0, AGENT_COUNT - 1);
			AAgent* temp = Agents[rand];
			Agents[rand] = Agents[rand2];
			Agents[rand2] = temp;
		}
	}
}
