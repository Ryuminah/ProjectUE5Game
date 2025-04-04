// Copyright Epic Games, Inc. All Rights Reserved.

#include "DataTableAutoGeneratorCommands.h"

#define LOCTEXT_NAMESPACE "FDataTableAutoGeneratorModule"

void FDataTableAutoGeneratorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "DTGenerator", "Bring up DataTableAutoGenerator window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
