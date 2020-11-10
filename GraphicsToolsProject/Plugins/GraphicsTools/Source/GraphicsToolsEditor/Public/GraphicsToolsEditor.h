// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(GraphicsToolsEditor, All, All)

/**
 * Entry and exit point for the Graphics Tools editor module.
 */
class FGraphicsToolsEditorModule : public IModuleInterface
{
public:
	/** Module entry point. */
	virtual void StartupModule() override;
	/** Module exit point. */
	virtual void ShutdownModule() override;
};
