// Copyright © 2023 AzureuBin. All rights reserved.

#include "QuickCommandStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

// 配合SlateStyleMacros使用
#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FQuickCommandStyle::StyleInstance = nullptr;

void FQuickCommandStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FQuickCommandStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FQuickCommandStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("QuickCommandStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FQuickCommandStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("QuickCommandStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("QuickCommand")->GetBaseDir() / TEXT("Resources"));

	Style->Set("QuickCommand.OpenQCMDPanel", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FQuickCommandStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FQuickCommandStyle::Get()
{
	return *StyleInstance;
}
