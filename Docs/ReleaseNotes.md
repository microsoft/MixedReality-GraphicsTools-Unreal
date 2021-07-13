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

### Android Support

Graphics Tools can now be used with on Android platforms. Testing is limited, but current tests pass on a Samsung Galaxy S20 5G.

### OpenXR Support

Graphics Tools now defaults to the OpenXR runtime. The Windows MR plugins is still supported. See the [OpenXR plugin](https://github.com/microsoft/Microsoft-OpenXR-Unreal) to learn more.

## Breaking changes

None

## Known issues

### Performance issues when updating the spatial mesh.

This is a know engine issue, a fix is slated for a future engine release.

If you run into issues caused by Graphics Tools or have questions about how to do something, please [file an issue](https://github.com/microsoft/MixedReality-GraphicsTools-Unreal/issues/new) on the GitHub repo.
