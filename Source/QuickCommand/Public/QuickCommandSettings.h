// Copyright © 2023 AzureuBin. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuickCommandTypes.h"
#include "Engine/DeveloperSettings.h"

#include "QuickCommandSettings.generated.h"

UCLASS(Config=Engine, DefaultConfig)
class UQuickCommandConfig : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	virtual FName GetCategoryName() const override;
	virtual FName GetSectionName() const override;
	virtual FName GetContainerName() const override;
	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostInitProperties() override;

	UPROPERTY(Config, EditAnywhere, Category=QuickCommandConfig)
	TArray<FConsoleVar> ConsoleVars;

	UPROPERTY(Config, EditAnywhere, Category=QuickCommandConfig)
	TArray<FConsoleCmd> ConsoleCmds;
};