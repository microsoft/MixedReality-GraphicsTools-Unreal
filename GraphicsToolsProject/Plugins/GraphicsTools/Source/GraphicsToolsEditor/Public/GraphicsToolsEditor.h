// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(GraphicsToolsEditor, All, All)

class FGraphicsToolsEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
