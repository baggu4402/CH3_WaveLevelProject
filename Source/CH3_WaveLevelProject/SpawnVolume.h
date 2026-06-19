// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class CH3_WAVELEVELPROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	TObjectPtr<UBoxComponent> SpawnBox;

	UPROPERTY(BlueprintReadOnly, Category = "Spawn")
	TArray<TObjectPtr<AActor>> SpawnedItems;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FVector GetRandomPointInVolume() const;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnItems(TSubclassOf<AActor> ItemClass, int32 Count);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void ClearSpawnedItems();
};
