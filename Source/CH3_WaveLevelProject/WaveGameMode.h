// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "WaveGameMode.generated.h"

class ASpawnVolume;
class UUserWidget;

USTRUCT(BlueprintType)
struct FWaveInfo
{
	GENERATED_BODY()

	FWaveInfo()
		: WaveNumber(1)
		, TimeLimit(30.0f)
		, SpawnCount(5)
	{
	}

	FWaveInfo(int32 InWaveNumber, float InTimeLimit, int32 InSpawnCount)
		: WaveNumber(InWaveNumber)
		, TimeLimit(InTimeLimit)
		, SpawnCount(InSpawnCount)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 WaveNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float TimeLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 SpawnCount;
};

UCLASS()
class CH3_WAVELEVELPROJECT_API AWaveGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWaveGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TArray<FWaveInfo> Waves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 CurrentLevelNumber;

	UPROPERTY(BlueprintReadOnly, Category = "Wave")
	int32 CurrentWaveIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AActor> SpawnItemClass;

	UPROPERTY(BlueprintReadOnly, Category = "Spawn")
	TObjectPtr<ASpawnVolume> SpawnVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TArray<FName> LevelNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<UUserWidget> HUDWidgetInstance;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void InitDefaultWaves();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void StartWave();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void TickWaveTimer();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void EndWave();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void MoveToNextWave();

	UFUNCTION(BlueprintCallable, Category = "Level")
	void MoveToNextLevel();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void HandleItemCollected();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void GameOver();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void GameClear();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void FindSpawnVolume();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void CreateHUD();

	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void ResetRunData();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void OnGameOver();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void OnGameClear();

protected:
	FTimerHandle WaveTimerHandle;
	FTimerHandle WaveTickTimerHandle;

	void RestoreRunDataFromGameInstance();
};
