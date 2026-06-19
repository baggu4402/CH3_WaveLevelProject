// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaveHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class CH3_WAVELEVELPROJECT_API UWaveHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHUD();

	UFUNCTION(BlueprintPure, Category = "HUD")
	FString FormatTime(float TimeSeconds) const;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Level;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Wave;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Time;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Score;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_HP;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_HP;
};
