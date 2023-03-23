// Copyright © 2023 AzureuBin. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "QuickCommandTypes.h"
#include "DetailCategoryBuilder.h"
#include "IDetailCustomization.h"

extern const FName QuickCommandTabName;

class FCommandDetail : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShareable(new FCommandDetail);
	}

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	
private:
	TSharedRef<SWidget> GenerateValueWidget(FConsoleVar& ConsoleVariable) const;
};

class FCommandPanel
{
public:
	TSharedRef<SDockTab> OnSpawnTab(const FSpawnTabArgs& Args) const;
	
private:
	TSharedRef<SWidget> GeneratePanelContent() const;
	TSharedRef<SWidget> GenerateDetailWidget() const;

	FReply OnReloadBtn() const;
	FReply OnSaveBtn() const;
	FReply OnClearBtn() const;

	TSharedPtr<SWindow> Window;
};
