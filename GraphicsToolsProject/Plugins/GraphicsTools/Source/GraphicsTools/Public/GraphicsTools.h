// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(GraphicsTools, All, All)

/**
 * Entry and exit point for the primary Graphics Tools module.
 */
class FGraphicsToolsModule : public IModuleInterface
{
public:
	/** Module entry point. */
	virtual void StartupModule() override;
	/** Module exit point. */
	virtual void ShutdownModule() override;
};
