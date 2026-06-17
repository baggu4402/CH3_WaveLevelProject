// Copyright Epic Games, Inc. All Rights Reserved.

#include "CH3_WaveLevelProjectPickUpComponent.h"

UCH3_WaveLevelProjectPickUpComponent::UCH3_WaveLevelProjectPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UCH3_WaveLevelProjectPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UCH3_WaveLevelProjectPickUpComponent::OnSphereBeginOverlap);
}

void UCH3_WaveLevelProjectPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ACH3_WaveLevelProjectCharacter* Character = Cast<ACH3_WaveLevelProjectCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
