// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveHUDWidget.h"
#include "WaveGameState.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"

void UWaveHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateHUD();
}

void UWaveHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateHUD();
}

void UWaveHUDWidget::UpdateHUD()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	AWaveGameState* WaveGameState = World->GetGameState<AWaveGameState>();
	if (!WaveGameState)
	{
		return;
	}

	if (Text_Level)
	{
		Text_Level->SetText(FText::FromString(FString::Printf(TEXT("LEVEL %d"), WaveGameState->CurrentLevel)));
	}

	if (Text_Wave)
	{
		Text_Wave->SetText(FText::FromString(FString::Printf(TEXT("WAVE %d"), WaveGameState->CurrentWave)));
	}

	if (Text_Time)
	{
		Text_Time->SetText(FText::FromString(FString::Printf(TEXT("TIME %s"), *FormatTime(WaveGameState->RemainingTime))));
	}

	if (Text_Score)
	{
		Text_Score->SetText(FText::FromString(FString::Printf(TEXT("SCORE %04d"), WaveGameState->Score)));
	}

	if (Text_HP)
	{
		Text_HP->SetText(FText::FromString(FString::Printf(TEXT("HP %.0f / %.0f"), WaveGameState->CurrentHP, WaveGameState->MaxHP)));
	}

	if (ProgressBar_HP)
	{
		const float HPPercent = WaveGameState->MaxHP > 0.0f ? WaveGameState->CurrentHP / WaveGameState->MaxHP : 0.0f;
		ProgressBar_HP->SetPercent(FMath::Clamp(HPPercent, 0.0f, 1.0f));
	}
}

FString UWaveHUDWidget::FormatTime(float TimeSeconds) const
{
	const int32 TotalSeconds = FMath::Max(0, FMath::CeilToInt(TimeSeconds));
	const int32 Minutes = TotalSeconds / 60;
	const int32 Seconds = TotalSeconds % 60;

	return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}
