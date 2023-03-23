// Copyright © 2023 AzureuBin. All rights reserved.


#include "QuickCommandTypes.h"

void FConsoleVar::UpdateValue()
{
	if (const IConsoleVariable* pConsoleVariable = GetConsolePtr())
	{
		Value = pConsoleVariable->GetString();
	}
	else
	{
		Value = TEXT("0");
	}
}

IConsoleVariable* FConsoleVar::GetConsolePtr() const
{
	return IConsoleManager::Get().FindConsoleVariable(*CVar);
}

void FConsoleVar::SetValue(bool NewValue)
{
	Value = NewValue ? TEXT("1") : TEXT("0");
	Internal_SetValue();
}

void FConsoleVar::SetValue(int32 NewValue)
{
	Value = FString::Printf(TEXT("%d"), NewValue);
	Internal_SetValue();
}

void FConsoleVar::SetValue(float NewValue)
{
	Value = FString::Printf(TEXT("%g"), NewValue);
	Internal_SetValue();
}

void FConsoleVar::SetValue(FString NewValue)
{
	Value = NewValue;
	Internal_SetValue();
}

void FConsoleVar::Internal_SetValue() const
{
	if (IConsoleVariable* pConsoleVariable = GetConsolePtr())
	{
		pConsoleVariable->Set(*Value, ECVF_SetByScalability);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" %s 不是控制台变量！"), *CVar);
	}
}
