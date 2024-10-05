

![Mixed Reality Toolkit](Docs/Images/Logos/MRTK_Unreal_GT_Banner_Rounded.png)

# What is Graphics Tools?

> [!IMPORTANT]
> Microsoft & Unreal have discontinued support for the Unreal Engine for HoloLens 2. Customers can continue using Unreal Engine to develop applications for HoloLens 2, but support has been discontinued (no bug fixes, content updates or technical support). Existing applications will continue to run on the last version of Unreal 5.1. Applications for HoloLens 2 can alternatively be [developed with Unity](https://learn.microsoft.com/windows/mixed-reality/develop/unity/unity-development-overview).

Graphics Tools for Unreal Engine is a UE game plugin with code, blueprints and example assets created to help improve the visual fidelity of Mixed Reality applications while staying within performance budgets.

| NOTE: Only HoloLens 2, Windows, and Android development is tested at the moment. If you encounter issues with other platforms please [file an issue](https://github.com/microsoft/MixedReality-GraphicsTools-Unreal/issues/new). |
| --- |

![Features](Docs/Images/Features.png)

# Getting started with Graphics Tools

If you're new to MRTK or Mixed Reality development in Unreal, **we recommend starting at the beginning of our** [Unreal development journey](https://docs.microsoft.com/windows/mixed-reality/unreal-development-overview) in the Microsoft Docs. The Unreal development journey is specifically tailored to walk new developers through the installation, core concepts, and usage of Unreal Engine for Mixed Reality.

If you're already familiar with Mixed Reality development in Unreal, you can find the installation instructions for the Graphics Tools plugin [here](Docs/Installation.md).

# Required software

 | [![Utilities](Docs/Images/Windows-Logo.png)](https://developer.microsoft.com/windows/downloads/windows-10-sdk) <br> [Windows SDK 18362+](https://developer.microsoft.com/windows/downloads/windows-10-sdk)| [![Unreal](Docs/Images/Unreal-Logo.png)](https://www.unrealengine.com/get-now) <br> [Unreal 5.1.0+](https://www.unrealengine.com/get-now)| [![Visual Studio 2019](Docs/Images/VS-Logo.png)](http://dev.windows.com/downloads) <br> [Visual Studio 2019](http://dev.windows.com/downloads)|
| :--- | :--- | :--- |
| To build apps with MRTK-Unreal and Graphics Tools, you need the Windows 10 May 2019 Update SDK | The Unreal Engine provides support for building Mixed Reality projects in Windows 10 | Visual Studio is used for code editing |

# Graphics building blocks

| [![Lighting](Docs/Images/FeatureCards/Lighting.png)](Docs/Lighting.md) [Lighting](Docs/Lighting.md) | [![Proximity Lights](Docs/Images/FeatureCards/ProximityLights.png)](Docs/ProximityLights.md) [Proximity Lights](Docs/ProximityLights.md) | [![Effects](Docs/Images/FeatureCards/Effects.png)](Docs/Effects.md) [Effects](Docs/Effects.md) |
|---|---|---|
| Physically based lighting technique optimized for Mixed Reality | A custom light type used to demonstrate distance to surface | Common graphics effects utilized in Mixed Reality |
[ ![ClippingPrimitives](Docs/Images/FeatureCards/ClippingPrimitives.png)](Docs/ClippingPrimitives.md) [Clipping Primitives](Docs/ClippingPrimitives.md) | [![Profiling](Docs/Images/FeatureCards/Profiling.png)](Docs/Profiling.md) [Profiling](Docs/Profiling.md) | [![SpatialPerception](Docs/Images/FeatureCards/SpatialPerception.png)](Docs/SpatialPerception.md) [Spatial Perception](Docs/SpatialPerception.md) |
| Tools to dynamically slice away a mesh and peer inside the geometry | Explore profiling techniques and tools useful for Mixed Reality development | Techniques that create visually compelling materials for real world geometry |
| [![MeshOutlines](Docs/Images/FeatureCards/MeshOutlines.png)](Docs/MeshOutlines.md) [Mesh Outlines](Docs/MeshOutlines.md) |   |   |
| Editor and runtime components to render an outline around a static mesh |  |  |

# Example levels

If you want to explore the different Graphics Tools features or want a reference for how to use them, we recommend having a look at the example levels contained in the _Graphics Tools Examples_ plugin (/GraphicsToolsProject/Plugins/GraphicsToolsExamples). You can copy the examples plugin (along with the GraphicsTools plugin) into the 'Plugins' subfolder of your own project or you can simply try out the _Graphics Tools Project_ (/GraphicsToolsProject) in this repository. For that you should:

1. [Clone](https://help.github.com/en/desktop/contributing-to-projects/cloning-a-repository-from-github-to-github-desktop) this repository.
1. [Checkout](https://help.github.com/en/desktop/contributing-to-projects/switching-between-branches) the latest 'public/_version_.x' branch.
    * Bear in mind that this branch is alive. It's not a release, and will be **updated regularly with potentially breaking changes**. There will be a release tag (e.g. _release/0.4.0_) marked as such in GitHub.

You can now open the _Graphics Tools Project_ (/GraphicsToolsProject) and explore individual example levels or open the _Loader_ level to access some of the examples from a centralized hub. You can also copy the examples (/GraphicsToolsProject/Plugins/GraphicsToolsExamples) into the 'Plugins' folder in your own project and use the pre-made elements as a base for your own Mixed Reality experience.

Please note that the example content might change significantly between releases. Please make sure to always run the examples with a matching version of the Graphics Tools Plugin as there might be breaking changes in the example code.

# Packaged Graphics Tools project

We also provide the _Graphics Tools Project_ (/GraphicsToolsProject) pre-packaged for HoloLens 2 so you can try out the main features directly on device easily. To use it:

1. Obtain the packaged game from the latest release page (e.g. _GTProject-HoloLens.0.4.0.zip_) and unzip it to a local directory.
1. Install it in the device via the [Device Portal](https://docs.microsoft.com/en-us/windows/uwp/debug-test-perf/device-portal#install-sideload-an-app).

# Videos

| | [![Working with UE and HoloLens 2](Docs/Images/Readme/UnrealAndHL_Webinar.png)](https://youtu.be/t95Bge-yk5E) | |
| :--- | :--- | :--- |
| | Webinar showing how to build a product configurator using Graphics Tools | |

# Feedback and contributions

This project welcomes contributions and suggestions.  Most contributions require you to agree to a
Contributor License Agreement (CLA) declaring that you have the right to, and actually do, grant us
the rights to use your contribution. For details, visit https://cla.opensource.microsoft.com.

When you submit a pull request, a CLA bot will automatically determine whether you need to provide
a CLA and decorate the PR appropriately (e.g., status check, comment). Simply follow the instructions
provided by the bot. You will only need to do this once across all repos using our CLA.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.
