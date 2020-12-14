---
title: ProximityLights
description: Guide to graphic resources and techniques in Graphics Tools.
author: Cameron-Micka
ms.author: thmicka
ms.date: 12/12/2020
ms.localizationpriority: high
keywords: Unreal, Unreal Engine, UE4, HoloLens, HoloLens 2, Mixed Reality, development, MRTK, GT, Graphics Tools, graphics, rendering, materials
---

# Proximity Lights

A Proximity Light is a [Fluent Design System](https://www.microsoft.com/design/fluent) paradigm that mimics a [point light](https://docs.unrealengine.com/en-US/BuildingWorlds/LightingAndShadows/LightTypes/Point/index.html) projected onto a surface with a color gradient. Often used for near interactions, an app can control the location and properties of a Proximity Light via the `GTProximityLight` component (or actor). To utilize Proximity Lights on a [material](https://docs.unrealengine.com/en-US/RenderingAndGraphics/Materials/index.html), place the `MF_GTProximityLights` [function](https://docs.unrealengine.com/en-US/RenderingAndGraphics/Materials/Functions/index.html) within a material and assign the output to the material's emissive color.

![Proximity Light](Images/ProximityLight/ProximityLight.png)

> [!NOTE] 
> Up to 3 Proximity Lights can effect a material at once. Additional Proximity Lights will not be included in light calculations.

Another unique feature of Proximity Lights is that they can pulse to draw attention. To pulse a Proximity Light call the `Pulse` member function via Blueprint or C++. 

## Example Level

There are some demonstrations of Proximity Lights within the `\GraphicsToolsProject\Plugins\GraphicsToolsExamples\Content\ProximityLights\ProximityLights.umap` level.

## Example Usage

In the following steps we will create a new material that is illuminated by a Proximity Light.

1. Right click within the "Content Browser" and select "Material" under the "Create Basic Asset" menu listings. Let's name our material `M_ProximityLit`. Double click on `M_ProximityLit` to open the material editor. 

2. It's good practice to keep your materials simple (in other words keep the number of shader instructions low) when authoring materials for Mixed Reality. To ensure this, mark `M_ProximityLit` as "Unlit" (1) in the material's "Shading Model" property. If you would like your material to still look as though it is lit, then right click on the material graph and add the `MF_GTDefaultLit` function. Connect the result of the `MF_GTDefaultLit` to the material's "Emissive Color." (2)
![Material Setup](Images/ProximityLight/ProximityLightMaterialSetup.png)

3. Our material now needs to add Proximity Light calculations. To do this right click on the material graph and add the `MF_GTProximityLights` function (1). Connect the result of the `MF_GTProximityLights` to the material's "Emissive Color." If your material is also using the `MF_GTDefaultLit` function (from step 3) then add the result from both functions before connecting the result to the material's "Emissive Color." (2)
![Material Light](Images/ProximityLight/ProximityLightMaterialLight.png)

4. Next let's give our material a base color. Very few materials exist in the real world are completely black (and completely black materials render transparently on additive displays like the one found on HoloLens). Right click on the material graph and add the `ConstantVector3` node. Set the node's RGB channels to 0.5, a neutral gray color. Connect this node to the "Base Color" input of the `MF_GTDefaultLit` function. (1)
![Material Light](Images/ProximityLight/ProximityLightMaterialColor.png)

5. It's time to preview our material interacting with a Proximity Light. First let's create a new level (File > New Level) and select a "Empty Level." From the "Place Actors" panel drop a "Sphere" actor into the level. With the `Sphere` actor selected change the material to our newly created material, `M_ProximityLit`. Scale the sphere down to about 25% so that it's not so large.
![Actor Setup](Images/ProximityLight/ProximityLightActorSetup.png)

6. You may notice now that our `Sphere` actor is completely black and can't be differentiated from the background. To fix this let's add a directional light to our scene. Because we are using the `MF_GTDefaultLit` function we must use a `GTDirectionalLight` actor (or component). From the "Place Actors" panel drop a `GTDirectionalLight` actor into the level. Set the `GTDirectionalLight`'s "Intensity" to 1. The default value represents a sunny day and may wash out our Proximity Light. More information about the `GTDirectionalLight` component can be found in the [lighting](Lighting.md) documentation.

7. Finally it's time to add a Proximity Light to the level. From the "Place Actors" panel drop a `GTProximityLight` actor into the level. Move the `GTProximityLight` actor around until it hovers near the surface of the the `Sphere` actor. You will now see the Proximity Light illuminate the surface with a blue-purple gradient. (1) Try experimenting with different Proximity Light colors by changing the Center, Middle, and Outer colors. Or, try changing the size of Proximity Light via the Near or Far radii. If you want to hide the Proximity Light without removing the actor you can uncheck the "Visible" flag under the "Rendering" properties. 
![Actor Complete](Images/ProximityLight/ProximityLightActorComplete.png)

## Advanced Usage

By default only three Proximity Lights can illuminate a material at a time. If your project requires an additional Proximity Light to influence a material the sample code below demonstrates how to achieve this.

> [!NOTE]
> Having many Proximity Lights illuminate a material will increase pixel shader instructions and will impact performance. Please profile these changes within your project.

*How to increase the number of available Proximity Lights from three to four.*

```C++
// 1) Within GraphicsToolsProject\Plugins\GraphicsTools\Source\GraphicsTools\Private\GTProximityLightComponent.cpp change:

#define GT_MAX_PROXIMITY_LIGHTS 3

// to:

#define GT_MAX_PROXIMITY_LIGHTS 4

// 2) Open GraphicsToolsProject\Plugins\GraphicsTools\Content\Materials\MF_GTProximityLights.uasset within the Unreal Editor. With the `Contribution Proximity Lights` custom expression selected change the "Additional Defines" property from:

GT_MAX_PROXIMITY_LIGHTS 3

// to:

GT_MAX_PROXIMITY_LIGHTS 4
```

To add support for more than 4 lights the above changes must be made as well as within the UpdateParameterCollection method in GTProximityLightComponent.cpp change each instance of ParameterNames (6 in total) to include a new entry. In the `MPC_GTSettings` [material parameter collection](https://docs.unrealengine.com/en-US/RenderingAndGraphics/Materials/ParameterCollections/index.html) add 6 new entries for all parameter names which were added above. In `GraphicsToolsProject\Plugins\GraphicsTools\Shaders\GTProximityLightingUnreal.ush` add additional function parameters to the `GTContributionProximityLights` function. Open `GraphicsToolsProject\Plugins\GraphicsTools\Content\Materials\MF_GTProximityLights.uasset` within the Unreal Editor and include the additional inputs in the `Contribution Proximity Lights` [custom expression](https://docs.unrealengine.com/en-US/RenderingAndGraphics/Materials/ExpressionReference/Custom/index.html). Finally connect those inputs to the corresponding `MPC_GTSettings` parameters.

## See also

- [Lighting](Lighting.md)
- [Effects](Effects.md)
