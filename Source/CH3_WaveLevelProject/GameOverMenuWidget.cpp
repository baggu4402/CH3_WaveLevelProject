// Fill out your copyright notice in the Description page of Project Settings.

#include "GameOverMenuWidget.h"
#include "WaveGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UGameOverMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Restart)
	{
		Button_Restart->OnClicked.AddDynamic(this, &UGameOverMenuWidget::OnRestartClicked);
	}

	if (Button_MainMenu)
	{
		Button_MainMenu->OnClicked.AddDynamic(this, &UGameOverMenuWidget::OnMainMenuClicked);
	}

	UpdateFinalScore();
}

void UGameOverMenuWidget::UpdateFinalScore()
{
	if (!Text_FinalScore)
	{
		return;
	}

	const UWaveGameInstance* WaveGameInstance = GetGameInstance<UWaveGameInstance>();
	const int32 FinalScore = WaveGameInstance ? WaveGameInstance->GetTotalScore() : 0;
	Text_FinalScore->SetText(FText::FromString(FString::Printf(TEXT("Score: %04d"), FinalScore)));
}

void UGameOverMenuWidget::OnRestartClicked()
{
	if (UWaveGameInstance* WaveGameInstance = GetGameInstance<UWaveGameInstance>())
	{
		WaveGameInstance->ResetRunData();
	}

	UGameplayStatics::OpenLevel(this, FName(TEXT("L_Level_01")));
}

void UGameOverMenuWidget::OnMainMenuClicked()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("L_MainMenu")));
}
