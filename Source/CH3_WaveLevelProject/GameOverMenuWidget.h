// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverMenuWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class CH3_WAVELEVELPROJECT_API UGameOverMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Game Over")
	void UpdateFinalScore();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Restart;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_MainMenu;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_FinalScore;

	UFUNCTION()
	void OnRestartClicked();

	UFUNCTION()
	void OnMainMenuClicked();
};
