// Fill out your copyright notice in the Description page of Project Settings.

#include "GameClearMenuWidget.h"
#include "WaveGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameClearMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_MainMenu)
	{
		Button_MainMenu->OnClicked.AddDynamic(this, &UGameClearMenuWidget::OnMainMenuClicked);
	}

	if (Button_Quit)
	{
		Button_Quit->OnClicked.AddDynamic(this, &UGameClearMenuWidget::OnQuitClicked);
	}

	UpdateFinalScore();
}

void UGameClearMenuWidget::UpdateFinalScore()
{
	if (!Text_FinalScore)
	{
		return;
	}

	const UWaveGameInstance* WaveGameInstance = GetGameInstance<UWaveGameInstance>();
	const int32 FinalScore = WaveGameInstance ? WaveGameInstance->GetTotalScore() : 0;
	Text_FinalScore->SetText(FText::FromString(FString::Printf(TEXT("Final Score: %04d"), FinalScore)));
}

void UGameClearMenuWidget::OnMainMenuClicked()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("L_MainMenu")));
}

void UGameClearMenuWidget::OnQuitClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}
