// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveGameInstance.h"
#include "Math/UnrealMathUtility.h"

UWaveGameInstance::UWaveGameInstance()
{
	TotalScore = 0;
	MaxHP = 100.0f;
	SavedHP = 100.0f;
}

void UWaveGameInstance::AddScore(int32 Amount)
{
	TotalScore += Amount;
}

void UWaveGameInstance::SetTotalScore(int32 NewScore)
{
	TotalScore = NewScore;
}

int32 UWaveGameInstance::GetTotalScore() const
{
	return TotalScore;
}

void UWaveGameInstance::SetSavedHP(float NewHP)
{
	SavedHP = FMath::Clamp(NewHP, 0.0f, MaxHP);
}

float UWaveGameInstance::GetSavedHP() const
{
	return SavedHP;
}

void UWaveGameInstance::ResetRunData()
{
	TotalScore = 0;
	SavedHP = MaxHP;
}
