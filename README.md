# Little Vulkan Engine

A video tutorial series introducing computer graphics for [Vulkan®](https://www.khronos.org/vulkan/), the new generation graphics and compute API from Khronos.

Currently this repository is more for use as a reference and for my own use to track the progress of this series, rather than something that can be cloned and used as a starting point, at least until I make a more robust build system.


## Table of Contents
+ [Building](#Building)
+ [Tutorials](#Tutorials)
    + [Basics](#Basics)
+ [Official Khronos Vulkan Samples](#Khronossamples)
+ [Credits and Attributions](#CreditsAttributions)

## <a name="Building"></a> Building

This repository is still in the early stages and doesn't contain everything you may need to compile and build the code on your machine. If you wish to use this as a starting point for your project my recommendation is to follow the tutorial series. Any contributions to making a more robust, multi-platform build system are appreciated :)

## <a name="Tutorials"></a> Tutorials

### <a name="Basics"></a> Basics

#### [00 - Starting Point](littleVulkanEngine/tutorial00/)

This is the staring point to the tutorial series. It is equivalent to where you'd be at if you completed the linux section of 
setting up your development environment as described by [Vulkan Tutorial](https://vulkan-tutorial.com/Development_environment) 
with just a few small changes.

  [Video Tutorial](https://youtu.be/Y9U9IE0gVHA)

#### [01 - Opening a window](littleVulkanEngine/tutorial01/)

In the very first tutorial of this series we will be creating a class to encapsulate a glfw window, and a class to run our application.

  [Video Tutorial](https://youtu.be/lr93-_cC8v4)

#### [02 - Shaders & the graphics pipeline](littleVulkanEngine/tutorial02/)

In this tutorial we create the most basic of vertex and fragment shaders files, go over how to compile them to SPIR-V, and read
the compiled files into our c++ program, to later be used to create shader modules in the next tutorial. 

  [Video Tutorial](https://youtu.be/_riranMmtvI)

#### [03 & 04 - Pipeline Creation & Vulkan Device](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial03)

In this tutorial we configure our graphics pipeline create info to prepare our pipeline for drawing. We read in the compiled shader files created in the previous tutorial and use them to create shader modules. We also have added the lve_device helper file which encapsulates a vulkan device and functionality for creating a vulkan instance, setting up validation layers, picking a physical device and creating a logical device and queues. Additionally, there is a bit of helper functionality.

[Tutorial 03 Video](https://youtu.be/LYKlEIzGmW4)
Tutorial 04 Video not yet released (est. December 18th)

#### [05 - Command Buffers & the Swap Chain](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial04)

We finally draw a triangle!! In this tutorial we cover command buffers and give a brief overview of the swap chain.

Video not yet released (est. December 23rd)

## <a name="Khronossamples"></a> Official Khronos Vulkan Samples

Khronos made an official Vulkan Samples repository available to the public ([press release](https://www.khronos.org/blog/vulkan-releases-unified-samples-repository?utm_source=Khronos%20Blog&utm_medium=Twitter&utm_campaign=Vulkan%20Repository)).

You can find this repository at https://github.com/KhronosGroup/Vulkan-Samples

## <a name="CreditsAttributions"></a> Credits
Thanks to the authors of these libraries :
- [OpenGL Mathematics (GLM)](https://github.com/g-truc/glm)

Thanks to [LunarG](http://www.lunarg.com)

Thanks to the wonderful opensource examples by [Sascha Willems](https://github.com/SaschaWillems/Vulkan)

Thanks to [ThinMatrix](https://www.youtube.com/user/ThinMatrix/featured) and his wonderful OpenGL game tutorial series which was a huge inspiration for this series and how I first started learning computer graphics

Thanks to [Sean Plott](https://day9.tv/) and the #DK30 challenge, for providing the motivating kick to give this a shot

## Attributions / Licenses
- Vulkan and the Vulkan logo are trademarks of the [Khronos Group Inc.](http://www.khronos.org)
