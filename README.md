# Little Vulkan Engine

A video tutorial series introducing computer graphics for [Vulkan®](https://www.khronos.org/vulkan/), the new generation graphics and compute API from Khronos.

Currently this repository is more for use as a reference and for my own use to track the progress of this series, rather than something that can be cloned and used as a starting point, at least until I make a more robust build system.

## Table of Contents

- [Building](#Building)
- [Tutorials](#Tutorials)
  - [Basics](#Basics)
- [Official Khronos Vulkan Samples](#Khronossamples)
- [Credits and Attributions](#CreditsAttributions)

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

#### [03 & 04 - Pipeline Creation & Vulkan Device](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial03%2604)

In this tutorial we configure our graphics pipeline create info to prepare our pipeline for drawing. We read in the compiled shader files created in the previous tutorial and use them to create shader modules. We also have added the lve_device helper file which encapsulates a vulkan device and functionality for creating a vulkan instance, setting up validation layers, picking a physical device and creating a logical device and queues. Additionally, there is a bit of helper functionality.

[Tutorial 03 Video](https://youtu.be/LYKlEIzGmW4)

[Tutorial 04 Video](https://youtu.be/ecMcXW6MSYU)

#### [05 - Command Buffers & the Swap Chain](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial05)

We finally draw a triangle!! In this tutorial we cover command buffers and give a brief overview of the swap chain, and the initial functionality we will be using to draw to a window.

[Video Tutorial - Part 1](https://youtu.be/IUYH74MqxOA)

[Video Tutorial - Part 2](https://youtu.be/_VOR6q3edig)

#### [06 - Vertex Buffers](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial06)

In this tutorial, we will cover how vertex buffers store data, and can be bound to graphics pipelines. A vertex buffer is really just a chunk of memory that we pass into our vertex shader. We can put whatever data we would like into this memory as long as we tell our graphics pipeline how it is structured.

[Tutorial 06 Video](https://www.youtube.com/watch?v=mnKp501RXDc)

#### [07 - Fragment Interpolation](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial07)

We update the vertex struct and buffer to include an additional color attribute. We then use the color attribute to demonstrate fragment interpolation for values output from the vertex shader to the fragment shader.

[Tutorial 07 Video](https://youtu.be/ngoZZkMuCOM)

#### [08 - Swap Chain Recreation & Dynamic Viewports](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial08)

We set up a callback function that listens to changes on the glfw window and updates the dimensions of the window wrapper class. When the window is resized we must create a new compatible swap chain.

[Tutorial 08 Video](https://youtu.be/0IIqvi3Z0ng)

#### [09 - Push Constants](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial09)

Vulkan Push constants are an easy and performant way to pass data to a shader, via a vulkan command buffer rather than writes to memory or copy commands. They’re a great option for data that updates frequently, however have a main drawback of being limited in size.

[Tutorial 09 Video](https://youtu.be/wlLGLWI9Fdc)

#### [10 - 2D Transformations](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial10)

Matrices are an incredibly useful tool for representing linear transformations on geometry. In this tutorial we cover 2d transformation matrices and use them in our shaders to stretch, shrink and rotate our vertex position.

[Tutorial 10 Video](https://www.youtube.com/watch?v=gxUcgc88tD4)

#### [11 - Renderer & Systems](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial11)

This is a coding focused tutorial with not much theory. We restructure our application class by creating a Renderer class and a SimpleRenderSystem class. This modular and flexible design should make implementing many future topics possible and easy to integrate into the engine.

[Tutorial 11 Video](https://youtu.be/uGRSTRGlZVs)

#### [12 - Homogeneous Coordinates & Euler Angles](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial12)

We finally moved to 3D! In this tutorial we update the engine to use 3D positions and homogeneous coordinates, opening the possibility to more complicated transformation matrices.

[Tutorial 12 Video](https://youtu.be/0X_kRtyVzm4)

#### [13 - Projection Matrices](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial13)

We implement functions to create the orthographic projection and the perspective projection matrices. Projection matrices change the shape and location of vulkan’s canonical view volume.

[Tutorial 13 Video](https://youtu.be/YO46x8fALzE)

#### [14 - The Camera Transform](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial14)

We implement the camera/view transform. This allows us to place a virtual camera anywhere within the game world, which makes it possible to control what is captured by the viewing frustum and displayed to the screen.

[Tutorial 14 Video](https://youtu.be/rvJHkYnAR3w)

#### [15 - Game Loops and User Input](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial15)

Getting user input and time loops are both integral parts to any game engine and are complex topics. In this tutorial I briefly introduce the concepts and we add the ability to dynamically move the camera in a frame rate independent way.

[Tutorial 15 Video](https://youtu.be/wFV9zPU_Cjg)

#### [16 - Index and Staging Buffers](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial16)

Index buffers are a way to reduce the amount of gpu memory required to store a model’s attribute data by allowing duplicate vertex data to be removed from the Vertex Buffer.

[Tutorial 16 Video](https://youtu.be/qxuvQVtehII)

#### [17 - Loading 3D Models](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial17)

In this tutorial we make use of tinyobjloader, a tiny but powerful single file wavefront obj loader, to load 3D models into the engine.

[Tutorial 17 Video](https://youtu.be/jdiPVfIHmEA)

#### [18 - Diffuse Shading](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial18)

We implement a diffuse lighting model with a constant ambient light term in the vertex shader. This colors the vase object by calculating the intensity of light for each vertex based on how directly the surface at the point faces the incoming light.

[Tutorial 18 Video](https://youtu.be/wfh2N4u-nOU)

#### [19 - Uniform Buffers](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial19)

We create a buffer abstraction to make working with buffers a bit more convenient. We replace existing usages (vertex and index buffers) with this new abstraction, and then create a uniform buffer object which will store read-only data that can be updated dynamically between frames, to pass additional data to our shaders.

[Tutorial 19 Video](https://youtu.be/may_GMkfs5k)

#### [20 - Descriptor Sets](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial20)

In this tutorial, we create abstractions for descriptor sets, descriptor set layouts and descriptor pools, to make these objects easier to work with within the engine. We create a descriptor set layout that contains a single buffer descriptor, and use this to bind the uboBuffer for use within the vertex shader.

[Tutorial 20 Video](https://youtu.be/d5p44idnZLQ)

### <a name="Point Light"></a> Point Lights

Find the [Preliminary Completed Project Here](https://github.com/blurrypiano/littleVulkanEngine/tree/pointLights/littleVulkanEngine/tutorial)

#### [21 - Intro to Point Lights](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial21)

In this tutorial we add a point light object to the global UBO and update the vertex shader to make use of this new lighting technique. ([Video](https://youtu.be/Z1lLwAEMt4M))

#### [22 - Vertex vs Fragment Lighting](https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/tutorial22)

In this tutorial we explore the differences in per-fragment versus per-vertex lighting ([Video](https://youtu.be/YnMyKHfrgU4))

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
