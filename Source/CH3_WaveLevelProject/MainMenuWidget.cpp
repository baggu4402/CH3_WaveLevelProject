// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "WaveGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Start)
	{
		Button_Start->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartClicked);
	}

	if (Button_Quit)
	{
		Button_Quit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
	}
}

void UMainMenuWidget::OnStartClicked()
{
	if (UWaveGameInstance* WaveGameInstance = GetGameInstance<UWaveGameInstance>())
	{
		WaveGameInstance->ResetRunData();
	}

	UGameplayStatics::OpenLevel(this, FName(TEXT("L_Level_01")));
}

void UMainMenuWidget::OnQuitClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}
