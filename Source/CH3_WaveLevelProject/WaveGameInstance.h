// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WaveGameInstance.generated.h"

UCLASS()
class CH3_WAVELEVELPROJECT_API UWaveGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UWaveGameInstance();

	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	int32 TotalScore;

	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	float SavedHP;

	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	float MaxHP;

	UFUNCTION(BlueprintCallable, Category = "Game Data")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Game Data")
	void SetTotalScore(int32 NewScore);

	UFUNCTION(BlueprintPure, Category = "Game Data")
	int32 GetTotalScore() const;

	UFUNCTION(BlueprintCallable, Category = "Game Data")
	void SetSavedHP(float NewHP);

	UFUNCTION(BlueprintPure, Category = "Game Data")
	float GetSavedHP() const;

	UFUNCTION(BlueprintCallable, Category = "Game Data")
	void ResetRunData();
};
