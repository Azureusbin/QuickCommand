// Copyright © 2023 AzureuBin. All rights reserved.

#pragma once

#include "Runtime/Launch/Resources/Version.h"

#include "CoreMinimal.h"
#include "Misc/Paths.h"
#include "Misc/ConfigCacheIni.h"
#include "QuickCommandTypes.generated.h"

UENUM()
enum class EQuickCommandType : uint8
{
	Bool	=	0,
	Integer	=	2,
	Float	=	4,
	String	=	8,
};

inline QUICKCOMMANDCORE_API FString GetGameConfigPath()
{
	FString GameConfig = FPaths::ProjectConfigDir() / TEXT("DefaultGame.ini");
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 0
	GameConfig = FConfigCacheIni::NormalizeConfigIniPath(GameConfig);
#endif
	return GameConfig;
}

USTRUCT(meta=(HiddenByDefault))
struct QUICKCOMMANDCORE_API FConsoleVar
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Console Variables")
	FString Name;

	UPROPERTY(EditAnywhere, Category="Console Variables")
	FString CVar;

	UPROPERTY(EditAnywhere, Category="Console Variables", meta=(DisplayName="Type"))
	EQuickCommandType CVarType = EQuickCommandType::Bool;

	UPROPERTY(EditAnywhere, meta=(
		EditConditionHides,
		EditCondition="CVarType == EQuickCommandType::Integer || CVarType == EQuickCommandType::Float"),
		Category="Console Variables")
	float Min = 0.f;

	UPROPERTY(EditAnywhere, meta=(
		EditConditionHides,
		EditCondition="CVarType == EQuickCommandType::Integer || CVarType == EQuickCommandType::Float"),
		Category="Console Variables")
	float Max = 0.f;

	UPROPERTY()
	FString Value;

	void UpdateValue();
	
	IConsoleVariable* GetConsolePtr() const;

	void SetValue(bool NewValue);
	void SetValue(int32 NewValue);
	void SetValue(float NewValue);
	void SetValue(FString NewValue);

private:
	void Internal_SetValue() const;
};

USTRUCT(meta=(HiddenByDefault))
struct QUICKCOMMANDCORE_API FConsoleCmd
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Console Commands")
	FString Name;

	UPROPERTY(EditAnywhere, Category="Console Commands")
	FString Command;
};