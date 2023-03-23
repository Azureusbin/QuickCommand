// Copyright © 2023 AzureuBin. All rights reserved.

#include "QuickCommand.h"

#include "ToolMenus.h"
#include "CommandPanel.h"
#include "QuickCommandStyle.h"
#include "QCM_UICommands.h"


const FName QuickCommandTabName("QuickCommandPanel");

#define LOCTEXT_NAMESPACE "FQuickCommandModule"

void FQuickCommandModule::StartupModule()
{
	PanelManager = MakeShareable(new FCommandPanel);
	
	FQuickCommandStyle::Initialize();
	FQuickCommandStyle::ReloadTextures();
	
	FQCM_UICommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FQCM_UICommands::Get().OpenQCMDPanel,
		FExecuteAction::CreateRaw(this, &FQuickCommandModule::TryOpenCommandPanel),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FQuickCommandModule::RegisterMenus));

	const FOnSpawnTab& Delegate = FOnSpawnTab::CreateRaw(PanelManager.Get(), &FCommandPanel::OnSpawnTab);
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(QuickCommandTabName, Delegate)
		.SetDisplayName(FText::FromString(TEXT("Quick Command Panel")))
		.SetAutoGenerateMenuEntry(false);
}

void FQuickCommandModule::ShutdownModule()
{

	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FQuickCommandStyle::Shutdown();
	FQCM_UICommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(QuickCommandTabName);
}

void FQuickCommandModule::TryOpenCommandPanel()
{
	FGlobalTabmanager::Get()->TryInvokeTab(FTabId(QuickCommandTabName));
}

void FQuickCommandModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	{	// 注册到Window下拉菜单
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
		FToolMenuSection& Section = Menu->AddSection("QuickCommand", FText::FromString("Quick Command"));
		Section.AddMenuEntryWithCommandList(FQCM_UICommands::Get().OpenQCMDPanel, PluginCommands);
	}

	{	// 注册菜单栏按钮
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
		FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FQCM_UICommands::Get().OpenQCMDPanel));
		Entry.SetCommandList(PluginCommands);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuickCommandModule, QuickCommand)