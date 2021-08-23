---
title: Release Notes
description: Release notes for the latest GT release.
author: cameron-micka
ms.author: thmicka
ms.date: 01/04/2021
ms.localizationpriority: high
keywords: Unreal, Unreal Engine, UE4, HoloLens, HoloLens 2, Mixed Reality, development, MRTK, GT, Graphics Tools, release notes
---

# Graphics Tools 0.3.0 release notes

- [What's New](#whats-new)
- [Breaking Changes](#breaking-changes)
- [Known Issues](#known-issues)

The third release of the Graphics Tools supports Windows, HoloLens 2, and Android, but many material/shader techniques should work on other platforms. Please see the [README](../README.md#graphics-building-blocks) page for a list of all features and building blocks.

## What's new

### Android support

Graphics Tools can now be used with on Android platforms. Testing is limited, but current tests pass on a Samsung Galaxy S20 5G.

### OpenXR support

Graphics Tools now defaults to the OpenXR runtime. The Windows MR plugins is still supported. See the [OpenXR plugin](https://github.com/microsoft/Microsoft-OpenXR-Unreal) to learn more.

### Mesh outlines

Editor and runtime components to render an outline around a static mesh.

[![MeshOutlines](Images/FeatureCards/MeshOutlines.png)](Docs/MeshOutlines.md)

### Visual profiler improvements

The visual profiler will now render on top of all other others to ensure it is never occluded when being used.

## Breaking changes

None

## Known issues

### Ambient cubemaps on materials may look more pixelated at higher roughness values. 

The ambient cubemaps included with the engine, and used for the examples, no longer use angular filtering. Which means they no longer look "blurry" at higher mip map chain values. Materials at high roughness values use textures higher up the mip map chain. The final fix introduces explicit angular filtering for ambient cubemaps (UE-118357) and is scheduled for 4.27.1.

### The Visual Profiler with the D3D12 RHI doesn't update GPU time.

The OpenXR plugin allows Hololens 2 to use the DirectX 12 RHI (render hardware interface) which [doesn't support GPU time queries](https://github.com/EpicGames/UnrealEngine/blob/99b6e203a15d04fc7bbbf554c421a985c1ccb8f1/Engine/Source/Runtime/D3D12RHI/Private/D3D12RHIPrivate.h#L249). At the moment GPU times will not be displayed on HoloLens 2 when using the OpenXR plugin.

### Performance issues when rendering the spatial mesh with the checker material.

This is a know 4.27.x engine issue, a fix is being investigated.

If you run into issues caused by Graphics Tools or have questions about how to do something, please [file an issue](https://github.com/microsoft/MixedReality-GraphicsTools-Unreal/issues/new) on the GitHub repo.
