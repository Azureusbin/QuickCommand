// Copyright © 2023 AzureuBin. All rights reserved.

#include "QCM_UICommands.h"

#define LOCTEXT_NAMESPACE "FQuickCommandModule"

void FQCM_UICommands::RegisterCommands()
{
	UI_COMMAND(OpenQCMDPanel, "QuickCommand", "Open Quick Command Panel", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
