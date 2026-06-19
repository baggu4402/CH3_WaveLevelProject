// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveGameMode.h"
#include "SpawnVolume.h"
#include "WaveGameInstance.h"
#include "WaveGameState.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

AWaveGameMode::AWaveGameMode()
{
	GameStateClass = AWaveGameState::StaticClass();
	CurrentLevelNumber = 1;
	CurrentWaveIndex = 0;
	bShowDebugMessages = false;

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

	SetGameInputMode();

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
	RestoreRunDataFromGameInstance();
	CreateHUD();
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
	if (bShowDebugMessages && GEngine)
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
			WaveGameState->DamagePlayer(50.0f);

			if (UWaveGameInstance* WaveGameInstance = GetGameInstance<UWaveGameInstance>())
			{
				WaveGameInstance->SetSavedHP(WaveGameState->CurrentHP);
			}
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
	if (UWaveGameInstance* WaveGameInstance = GetGameInstance<UWaveGameInstance>())
	{
		WaveGameInstance->AddScore(10);
		WaveGameState->Score = WaveGameInstance->GetTotalScore();
	}

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
	if (bShowDebugMessages && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Game Over"));
	}

	ShowGameOverMenu();
	OnGameOver();
}

void AWaveGameMode::GameClear()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	GetWorldTimerManager().ClearTimer(WaveTickTimerHandle);

	UE_LOG(LogTemp, Warning, TEXT("Game Clear"));
	if (bShowDebugMessages && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Game Clear"));
	}

	ShowGameClearMenu();
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

void AWaveGameMode::CreateHUD()
{
	if (HUDWidgetInstance)
	{
		return;
	}

	if (!HUDWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUDWidgetClass is not set."));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD could not be created because World is invalid."));
		return;
	}

	HUDWidgetInstance = CreateWidget<UUserWidget>(World, HUDWidgetClass);
	if (!HUDWidgetInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create HUD widget."));
		return;
	}

	HUDWidgetInstance->AddToViewport();
}

void AWaveGameMode::ResetRunData()
{
	UWaveGameInstance* WaveGameInstance = GetGameInstance<UWaveGameInstance>();
	if (WaveGameInstance)
	{
		WaveGameInstance->ResetRunData();
	}

	AWaveGameState* WaveGameState = GetGameState<AWaveGameState>();
	if (WaveGameState)
	{
		WaveGameState->Score = 0;
		WaveGameState->MaxHP = WaveGameInstance ? WaveGameInstance->MaxHP : WaveGameState->MaxHP;
		WaveGameState->SetHP(WaveGameState->MaxHP);
	}
}

void AWaveGameMode::RestoreRunDataFromGameInstance()
{
	UWaveGameInstance* WaveGameInstance = GetGameInstance<UWaveGameInstance>();
	AWaveGameState* WaveGameState = GetGameState<AWaveGameState>();
	if (!WaveGameInstance || !WaveGameState)
	{
		return;
	}

	WaveGameState->Score = WaveGameInstance->GetTotalScore();
	WaveGameState->MaxHP = WaveGameInstance->MaxHP;
	WaveGameState->SetHP(WaveGameInstance->GetSavedHP());
}

void AWaveGameMode::ShowGameOverMenu()
{
	if (GameOverWidgetInstance)
	{
		SetMenuInputMode();
		return;
	}

	if (!GameOverWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameOverWidgetClass is not set."));
		return;
	}

	GameOverWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
	if (!GameOverWidgetInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create GameOver widget."));
		return;
	}

	GameOverWidgetInstance->AddToViewport();
	SetMenuInputMode();
}

void AWaveGameMode::ShowGameClearMenu()
{
	if (GameClearWidgetInstance)
	{
		SetMenuInputMode();
		return;
	}

	if (!GameClearWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameClearWidgetClass is not set."));
		return;
	}

	GameClearWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), GameClearWidgetClass);
	if (!GameClearWidgetInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create GameClear widget."));
		return;
	}

	GameClearWidgetInstance->AddToViewport();
	SetMenuInputMode();
}

void AWaveGameMode::SetMenuInputMode()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController)
	{
		return;
	}

	FInputModeUIOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
}

void AWaveGameMode::SetGameInputMode()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController)
	{
		return;
	}

	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;
}
