# Uniform Buffers - Tutorial 19

In this tutorial we create a buffer abstraction to make working with buffers a bit more convenient. We replace existing usages (vertex and index buffers) with this new abstraction, and then create a uniform buffer object which will store read-only data that can be updated dynamically between frames, to pass additional data to our shaders.

Uniform buffer data can be used within shaders, similarly to how to push constant data may be used. However, unlike push constant data that is limited to only 128 bytes for certain devices, the minimum guaranteed size for uniform buffer data is 16KB.

[View Tutorial Changes](https://github.com/blurrypiano/littleVulkanEngine/commit/0c1fd4dd97e5e8ab7cdf60ccafe23ed8e469405a)

[Video Tutorial](https://youtu.be/may_GMkfs5k)

[Buffer Header](https://pastebin.com/EcB0VmnE)

[Buffer Implementation](https://pastebin.com/3WAe7nJx)

## BUG FIX! - nonCoherentAtomSize

[Bug fix code changes](https://github.com/blurrypiano/littleVulkanEngine/commit/65b9c745d4e6728f8e61cd475c6ad72ac48a1ccd)

[Video - Bug fix](https://youtu.be/hFcmtJG3_Ao)

## Resources

[Zeux - Writing an efficient vulkan Renderer](https://zeux.io/2020/02/27/writing-an-efficient-vulkan-renderer/)

[Kyle Halladay - Comparing Uniform Data Transfer Methods](http://kylehalladay.com/blog/tutorial/vulkan/2017/08/13/Vulkan-Uniform-Buffers.html)

[Sascha Willems - Example VkBuffer wrapper class](https://github.com/SaschaWillems/Vulkan/blob/master/base/VulkanBuffer.h)

## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations.

Build the project using make, and run. The project should compile successfully and displaytwo vases. Move the camera with the arrow and WASD keys.

## Fixing Common Issues

The most likely cause for an error is that the relative filepaths are not correct. This can happen if the current working directory for when the program executable is run is not the same as the project directory. In this case try changing "shaders/simple_shader.vert.spv" as "shaders/simple_shader.frag.spv" in simple_render_system.cpp to use an absolute path instead, as well the filepath for "models/smooth_vase.obj" in first_app.cpp
