// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveGameMode.h"
#include "SpawnVolume.h"
#include "WaveGameState.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

AWaveGameMode::AWaveGameMode()
{
	GameStateClass = AWaveGameState::StaticClass();
	CurrentLevelNumber = 1;
	CurrentWaveIndex = 0;

	LevelNames.Add(TEXT("L_Level_01"));
	LevelNames.Add(TEXT("L_Level_02"));
	LevelNames.Add(TEXT("L_Level_03"));

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	if (PlayerPawnClassFinder.Succeeded())
	{
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	}
}

void AWaveGameMode::BeginPlay()
{
	Super::BeginPlay();

	const FString MapName = GetWorld() ? GetWorld()->GetMapName() : FString();
	if (MapName.Contains(TEXT("L_Level_01")))
	{
		CurrentLevelNumber = 1;
	}
	else if (MapName.Contains(TEXT("L_Level_02")))
	{
		CurrentLevelNumber = 2;
	}
	else if (MapName.Contains(TEXT("L_Level_03")))
	{
		CurrentLevelNumber = 3;
	}

	CurrentWaveIndex = 0;

	InitDefaultWaves();
	FindSpawnVolume();
	StartWave();
}

void AWaveGameMode::InitDefaultWaves()
{
	if (!Waves.IsEmpty())
	{
		return;
	}

	Waves.Add(FWaveInfo(1, 30.0f, 5));
	Waves.Add(FWaveInfo(2, 25.0f, 8));
	Waves.Add(FWaveInfo(3, 20.0f, 12));
}

void AWaveGameMode::StartWave()
{
	if (!Waves.IsValidIndex(CurrentWaveIndex))
	{
		MoveToNextLevel();
		return;
	}

	const FWaveInfo& CurrentWaveInfo = Waves[CurrentWaveIndex];
	AWaveGameState* WaveGameState = GetGameState<AWaveGameState>();
	if (WaveGameState)
	{
		WaveGameState->CurrentLevel = CurrentLevelNumber;
		WaveGameState->CurrentWave = CurrentWaveInfo.WaveNumber;
		WaveGameState->RemainingTime = CurrentWaveInfo.TimeLimit;
		WaveGameState->SetRemainingItemCount(CurrentWaveInfo.SpawnCount);
	}

	if (SpawnVolume)
	{
		SpawnVolume->ClearSpawnedItems();
		if (SpawnItemClass)
		{
			SpawnVolume->SpawnItems(SpawnItemClass, CurrentWaveInfo.SpawnCount);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SpawnItemClass is not set."));
		}
	}

	const FString Message = FString::Printf(TEXT("Wave %d Start!"), CurrentWaveInfo.WaveNumber);
	UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, Message);
	}

	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	GetWorldTimerManager().ClearTimer(WaveTickTimerHandle);
	GetWorldTimerManager().SetTimer(WaveTickTimerHandle, this, &AWaveGameMode::TickWaveTimer, 1.0f, true);
}

void AWaveGameMode::TickWaveTimer()
{
	AWaveGameState* WaveGameState = GetGameState<AWaveGameState>();
	if (!WaveGameState)
	{
		return;
	}

	WaveGameState->RemainingTime = FMath::Max(0.0f, WaveGameState->RemainingTime - 1.0f);
	if (WaveGameState->RemainingTime <= 0.0f)
	{
		EndWave();
	}
}

void AWaveGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(WaveTickTimerHandle);

	AWaveGameState* WaveGameState = GetGameState<AWaveGameState>();
	if (WaveGameState)
	{
		if (WaveGameState->RemainingItemCount > 0)
		{
			WaveGameState->DamagePlayer(20.0f);
		}

		if (WaveGameState->IsPlayerDead())
		{
			GameOver();
			return;
		}
	}

	MoveToNextWave();
}

void AWaveGameMode::MoveToNextWave()
{
	++CurrentWaveIndex;
	if (CurrentWaveIndex < Waves.Num())
	{
		StartWave();
		return;
	}

	MoveToNextLevel();
}

void AWaveGameMode::MoveToNextLevel()
{
	if (CurrentLevelNumber >= LevelNames.Num())
	{
		GameClear();
		return;
	}

	const FName NextLevelName = LevelNames[CurrentLevelNumber];
	UGameplayStatics::OpenLevel(this, NextLevelName);
}

void AWaveGameMode::HandleItemCollected()
{
	AWaveGameState* WaveGameState = GetGameState<AWaveGameState>();
	if (!WaveGameState)
	{
		return;
	}

	WaveGameState->AddScore(10);
	WaveGameState->DecreaseRemainingItemCount();

	if (WaveGameState->RemainingItemCount <= 0)
	{
		EndWave();
	}
}

void AWaveGameMode::GameOver()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	GetWorldTimerManager().ClearTimer(WaveTickTimerHandle);

	UE_LOG(LogTemp, Warning, TEXT("Game Over"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Game Over"));
	}

	OnGameOver();
}

void AWaveGameMode::GameClear()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	GetWorldTimerManager().ClearTimer(WaveTickTimerHandle);

	UE_LOG(LogTemp, Warning, TEXT("Game Clear"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Game Clear"));
	}

	OnGameClear();
}

void AWaveGameMode::FindSpawnVolume()
{
	SpawnVolume = nullptr;

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (TActorIterator<ASpawnVolume> It(World); It; ++It)
	{
		SpawnVolume = *It;
		break;
	}

	if (!SpawnVolume)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnVolume was not found in the level."));
	}
}
