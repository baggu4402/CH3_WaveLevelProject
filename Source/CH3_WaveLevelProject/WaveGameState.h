// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WaveGameState.generated.h"


UCLASS()
class CH3_WAVELEVELPROJECT_API AWaveGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AWaveGameState();

	UPROPERTY(BlueprintReadWrite, Category = "Wave")
	int32 CurrentLevel;

	UPROPERTY(BlueprintReadWrite, Category = "Wave")
	int32 CurrentWave;

	UPROPERTY(BlueprintReadWrite, Category = "Wave")
	float RemainingTime;

	UPROPERTY(BlueprintReadWrite, Category = "Wave")
	int32 Score;

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	float MaxHP;

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	float CurrentHP;

	UPROPERTY(BlueprintReadWrite, Category = "Wave")
	int32 RemainingItemCount;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void SetRemainingItemCount(int32 Count);

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void DecreaseRemainingItemCount();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetHP(float NewHP);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void DamagePlayer(float DamageAmount);

	UFUNCTION(BlueprintPure, Category = "Player")
	bool IsPlayerDead() const;
};
