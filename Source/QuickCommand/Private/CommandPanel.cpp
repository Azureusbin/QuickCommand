// Copyright © 2023 AzureuBin. All rights reserved.

#include "CommandPanel.h"

#include "DetailWidgetRow.h"
#include "DetailLayoutBuilder.h"
#include "Widgets/Input/SNumericEntryBox.h"

#include "QuickCommandSettings.h"

void FCommandDetail::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	// 隐藏Category
	DetailBuilder.HideCategory("QuickCommandConfig");

	UQuickCommandConfig* Config = GetMutableDefault<UQuickCommandConfig>();
	IDetailCategoryBuilder& VariableCategory = DetailBuilder.EditCategory("ConsoleVariables", FText::FromString(TEXT("Console Variables")) );
	IDetailCategoryBuilder& CommandCategory = DetailBuilder.EditCategory("ConsoleCommands", FText::FromString(TEXT("Console Commands")) );

	// 为每条Console Variable添加单独的一行
	for (FConsoleVar& ConsoleVariable : Config->ConsoleVars)
	{
		VariableCategory.AddCustomRow(FText::GetEmpty())
		.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString(ConsoleVariable.Name))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent()
		[
			GenerateValueWidget(ConsoleVariable)
		];
	}

	// 为每条Console Command添加单独的一行
	for (const FConsoleCmd& ConsoleCmd : Config->ConsoleCmds)
	{
		CommandCategory.AddCustomRow(FText::GetEmpty())
		.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString(ConsoleCmd.Name))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent()
		[
			SNew(SButton)
			.Text(FText::FromString(ConsoleCmd.Name))
			.OnClicked_Lambda([ConsoleCmd]()
			{
				GEngine->Exec(nullptr, *ConsoleCmd.Command);
				return FReply::Handled();
			})
		];
	}
	
}

TSharedRef<SWidget> FCommandDetail::GenerateValueWidget(FConsoleVar& ConsoleVariable) const
{
	const bool bLimitScope = (ConsoleVariable.Max!=ConsoleVariable.Min)&&(ConsoleVariable.Max>ConsoleVariable.Min);
	
	if (ConsoleVariable.CVarType == EQuickCommandType::Bool)
	{
		const bool Value = ConsoleVariable.Value.ToBool();
		return SNew(SCheckBox)
				.IsChecked(Value ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.OnCheckStateChanged_Lambda([&ConsoleVariable](ECheckBoxState InNewState)
				{
					const bool NewValue = ECheckBoxState::Checked==InNewState;
					ConsoleVariable.SetValue(NewValue);
				});
	}
	if (ConsoleVariable.CVarType == EQuickCommandType::Float)
	{
		const float Value = FCString::Atof(*ConsoleVariable.Value);
		return SNew(SSpinBox<float>)
				.Delta(0.01)
				.MaxValue(bLimitScope ? ConsoleVariable.Max : TOptional<float>())
				.MinValue(bLimitScope ? ConsoleVariable.Min : TOptional<float>())
				.Value(Value)
				.OnValueChanged_Lambda([&ConsoleVariable](float NewValue)
				{
					ConsoleVariable.SetValue(NewValue);
				});
	}
	if (ConsoleVariable.CVarType == EQuickCommandType::Integer)
	{
		const int32 Value = FCString::Atoi(*ConsoleVariable.Value);
		return SNew(SSpinBox<int32>)
				.Delta(1)
				.MaxValue(bLimitScope ? ConsoleVariable.Max : TOptional<int32>())
				.MinValue(bLimitScope ? ConsoleVariable.Min : TOptional<int32>())
				.Value(Value)
				.OnValueChanged_Lambda([&ConsoleVariable](int32 NewValue)
				{
					ConsoleVariable.SetValue(NewValue);
				});
	}
	
	// 处理string
	const FText Str = FText::FromString(ConsoleVariable.Value);
	return SNew(SEditableTextBox)
			.Text(Str)
			.OnTextCommitted_Lambda([&ConsoleVariable](const FText& Val, ETextCommit::Type TextCommitType)
			{
				ConsoleVariable.SetValue(Val.ToString());
			});
}

TSharedRef<SDockTab> FCommandPanel::OnSpawnTab(const FSpawnTabArgs& Args) const
{
	TSharedRef<SDockTab> Tab = SNew(SDockTab)
	.ShouldAutosize(false)
	.TabRole(NomadTab)
	[
		GeneratePanelContent()
	];

	return Tab;
}

TSharedRef<SWidget> FCommandPanel::GeneratePanelContent() const
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(3,5,3,1)
		.FillHeight(1)
		[
			GenerateDetailWidget()
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Padding(5)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5, 2, 5, 2)
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Reload")))
				.OnClicked_Raw(this, &FCommandPanel::OnReloadBtn)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5, 2, 5, 2)
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Save")))
				.OnClicked_Raw(this, &FCommandPanel::OnSaveBtn)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5, 2, 5, 2)
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Clear")))
				.OnClicked_Raw(this, &FCommandPanel::OnClearBtn)
			]
		   ];
}

TSharedRef<SWidget> FCommandPanel::GenerateDetailWidget() const
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.bShowOptions = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	TSharedRef<IDetailsView> DetailViewWidget = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	
	// 配置Detail View，使用我们自定义的IDetailCustomization
	DetailViewWidget->RegisterInstancedCustomPropertyLayout(UQuickCommandConfig::StaticClass(), FOnGetDetailCustomizationInstance::CreateStatic(&FCommandDetail::MakeInstance));
	DetailViewWidget->SetObject(GetMutableDefault<UQuickCommandConfig>());
	
	return DetailViewWidget;
}

FReply FCommandPanel::OnReloadBtn() const
{
	TArray<FConsoleVar>& ConsoleVars = GetMutableDefault<UQuickCommandConfig>()->ConsoleVars;
	for (FConsoleVar& ConsoleVar : ConsoleVars)
	{
		ConsoleVar.UpdateValue();
	}

	if (const TSharedPtr<SDockTab> Tab = FGlobalTabmanager::Get()->FindExistingLiveTab(QuickCommandTabName); Tab.IsValid())
	{
		Tab->SetContent(GeneratePanelContent());
	}
	
	return FReply::Handled();
}

FReply FCommandPanel::OnSaveBtn() const
{
	// Save console variables
	const FString GameConfig = GetGameConfigPath();
	TArray<FConsoleVar>& ConsoleVars = GetMutableDefault<UQuickCommandConfig>()->ConsoleVars;
	for (const FConsoleVar& ConsoleVar : ConsoleVars)
	{
		GConfig->SetString(TEXT("Quick Command"), *ConsoleVar.CVar, *ConsoleVar.Value, GameConfig);
	}
	GConfig->Flush(false, GameConfig);
	
	return FReply::Handled();
}

FReply FCommandPanel::OnClearBtn() const
{
	const FString GameConfig = GetGameConfigPath();
	
	if (GConfig->EmptySection(TEXT("Quick Command"), GameConfig))
	{
		GConfig->Flush(true, GameConfig);
	}
	
	return FReply::Handled();
}
