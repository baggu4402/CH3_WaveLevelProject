// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameClearMenuWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class CH3_WAVELEVELPROJECT_API UGameClearMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Game Clear")
	void UpdateFinalScore();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_MainMenu;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Quit;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_FinalScore;

	UFUNCTION()
	void OnMainMenuClicked();

	UFUNCTION()
	void OnQuitClicked();
};
