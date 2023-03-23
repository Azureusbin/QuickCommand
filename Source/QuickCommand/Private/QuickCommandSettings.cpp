// Copyright © 2023 AzureuBin. All rights reserved.

#include "QuickCommandSettings.h"


FName UQuickCommandConfig::GetCategoryName() const
{
	return TEXT("Plugins");
}

FName UQuickCommandConfig::GetSectionName() const
{
	return TEXT("QuickCMD");
}

FName UQuickCommandConfig::GetContainerName() const
{
	return TEXT("Project");
}

FText UQuickCommandConfig::GetSectionText() const
{
	return FText::FromString(TEXT("Quick Command"));
}

FText UQuickCommandConfig::GetSectionDescription() const
{
	return FText::FromString(TEXT("在此定义你的命令"));
}

void UQuickCommandConfig::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	for (FConsoleVar& ConsoleVar : ConsoleVars)
	{
		ConsoleVar.UpdateValue();
	}
}

void UQuickCommandConfig::PostInitProperties()
{
	Super::PostInitProperties();
		
	for (FConsoleVar& ConsoleVar : ConsoleVars)
	{
		ConsoleVar.UpdateValue();
	}
}
