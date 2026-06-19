// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	RootComponent = SpawnBox;
	SpawnBox->SetBoxExtent(FVector(500.0f, 500.0f, 100.0f));
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	const FVector Origin = SpawnBox ? SpawnBox->GetComponentLocation() : GetActorLocation();
	const FVector Extent = SpawnBox ? SpawnBox->GetScaledBoxExtent() : FVector::ZeroVector;

	return FVector(
		FMath::FRandRange(Origin.X - Extent.X, Origin.X + Extent.X),
		FMath::FRandRange(Origin.Y - Extent.Y, Origin.Y + Extent.Y),
		FMath::FRandRange(Origin.Z - Extent.Z, Origin.Z + Extent.Z));
}

void ASpawnVolume::SpawnItems(TSubclassOf<AActor> ItemClass, int32 Count)
{
	if (!ItemClass || Count <= 0)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (int32 Index = 0; Index < Count; ++Index)
	{
		const FVector SpawnLocation = GetRandomPointInVolume();
		AActor* SpawnedItem = World->SpawnActor<AActor>(ItemClass, SpawnLocation, FRotator::ZeroRotator);
		if (SpawnedItem)
		{
			SpawnedItems.Add(SpawnedItem);
		}
	}
}

void ASpawnVolume::ClearSpawnedItems()
{
	for (const TObjectPtr<AActor>& SpawnedItem : SpawnedItems)
	{
		if (IsValid(SpawnedItem))
		{
			SpawnedItem->Destroy();
		}
	}

	SpawnedItems.Empty();
}
