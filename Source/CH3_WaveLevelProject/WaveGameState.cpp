// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveGameState.h"
#include "Math/UnrealMathUtility.h"

AWaveGameState::AWaveGameState()
{
	CurrentLevel = 1;
	CurrentWave = 1;
	RemainingTime = 30.0f;
	Score = 0;
	MaxHP = 100.0f;
	CurrentHP = 100.0f;
	RemainingItemCount = 0;
}

void AWaveGameState::AddScore(int32 Amount)
{
	Score += Amount;
}

void AWaveGameState::SetRemainingItemCount(int32 Count)
{
	RemainingItemCount = FMath::Max(0, Count);
}

void AWaveGameState::DecreaseRemainingItemCount()
{
	RemainingItemCount = FMath::Max(0, RemainingItemCount - 1);
}

void AWaveGameState::SetHP(float NewHP)
{
	CurrentHP = FMath::Clamp(NewHP, 0.0f, MaxHP);
}

void AWaveGameState::DamagePlayer(float DamageAmount)
{
	SetHP(CurrentHP - DamageAmount);
}

bool AWaveGameState::IsPlayerDead() const
{
	return CurrentHP <= 0.0f;
}
