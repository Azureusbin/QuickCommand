// Copyright © 2023 AzureuBin. All rights reserved.

#include "QuickCommandCore.h"

#include "QuickCommandTypes.h"
#include "HAL/IConsoleManager.h"

#define LOCTEXT_NAMESPACE "FQuickCommandCoreModule"

void FQuickCommandCoreModule::StartupModule()
{
	LoadVariables();
}

void FQuickCommandCoreModule::ShutdownModule()
{
    
}

void FQuickCommandCoreModule::LoadVariables()
{
	TArray<FString> Variables;
	const FString GameConfig = GetGameConfigPath();
	GConfig->GetSection(TEXT("Quick Command"), Variables, GameConfig);

	for (FString Variable : Variables)
	{
		TArray<FString> Command;
		Variable.ParseIntoArray(Command, TEXT("="));

		if (!Command.IsEmpty() && Command.Num() == 2)
		{
			if (IConsoleVariable* ConsoleVariable = IConsoleManager::Get().FindConsoleVariable(*Command[0]))
			{
				ConsoleVariable->Set(*Command[1], ECVF_SetByScalability);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FQuickCommandCoreModule, QuickCommandCore)