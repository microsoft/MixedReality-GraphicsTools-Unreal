// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GraphicsTools.h"

#include "ShaderCore.h"

#include "Features/IModularFeatures.h"
#include "Interfaces/IPluginManager.h"

DEFINE_LOG_CATEGORY(GraphicsTools)

#define LOCTEXT_NAMESPACE "FGraphicsToolsModule"

void FGraphicsToolsModule::StartupModule()
{
	// Maps virtual shader source directory /Plugin/GraphicsTools to the plugin's actual Shaders directory.
	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("GraphicsTools"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Plugin/GraphicsTools"), PluginShaderDir);
}

void FGraphicsToolsModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGraphicsToolsModule, GraphicsTools)