---
title: Release Notes
description: Release notes for the latest GT release.
author: cameron-micka
ms.author: thmicka
ms.date: 01/04/2021
ms.localizationpriority: high
keywords: Unreal, Unreal Engine, UE4, HoloLens, HoloLens 2, Mixed Reality, development, MRTK, GT, Graphics Tools, release notes
---

# Graphics Tools 0.4.0 release notes

- [What's New](#whats-new)
- [Breaking Changes](#breaking-changes)
- [Known Issues](#known-issues)

The forth release of the Graphics Tools supports Windows, HoloLens 2, and Android, but many material/shader techniques should work on other platforms. Please see the [README](../README.md#graphics-building-blocks) page for a list of all features and building blocks.

## What's new

### Unreal Engine 5 support

Graphics Tools can now be used with Unreal Engine 5.

## Breaking changes

The plugin now assumes Unreal Engine 5 usage. For Unreal Engine 4 support please use [previous releases](https://github.com/microsoft/MixedReality-GraphicsTools-Unreal/releases) of Graphics Tools. 

## Known issues

### The Visual Profiler's text looks blurry with TAA

Text may not be readable when using temporal anti-aliasing (TAA). TAA is not supported on HoloLens 2 so text should be readable on device. 

