// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTWorldSubsystem.h"

bool UGTWorldSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return (UWorldSubsystem::DoesSupportWorldType(WorldType) || WorldType == EWorldType::EditorPreview);
}
