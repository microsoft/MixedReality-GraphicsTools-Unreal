// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(GraphicsTools, All, All)

/**
 * TODO
 */
class FGraphicsToolsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
