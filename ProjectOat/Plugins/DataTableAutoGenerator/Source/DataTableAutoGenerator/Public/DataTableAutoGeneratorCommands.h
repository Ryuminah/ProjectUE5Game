// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "DataTableAutoGeneratorStyle.h"

class FDataTableAutoGeneratorCommands : public TCommands<FDataTableAutoGeneratorCommands>
{
public:
	FDataTableAutoGeneratorCommands()
		: TCommands<FDataTableAutoGeneratorCommands>(TEXT("DataTableAutoGenerator"), NSLOCTEXT("Contexts", "DataTableAutoGenerator", "DataTableAutoGenerator Plugin"), NAME_None, FDataTableAutoGeneratorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> OpenPluginWindow;
	
	
};
