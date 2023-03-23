// Copyright © 2023 AzureuBin. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "QuickCommandStyle.h"

class FQCM_UICommands : public TCommands<FQCM_UICommands>
{
public:

	FQCM_UICommands()
		: TCommands<FQCM_UICommands>(TEXT("QuickCommand"),
			NSLOCTEXT("Contexts", "QuickCommand", "QuickCommand Plugin"),
			NAME_None, FQuickCommandStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenQCMDPanel;
};
