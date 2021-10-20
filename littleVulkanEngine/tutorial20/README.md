# Descriptor Sets - Tutorial 20

Vulkan uses descriptors as a way to provide resources to our shader programs.  Descriptors cannot be bound to a pipeline individually, and must be grouped into a set. Additionally, before pipeline creation, a descriptor set layout must be provided. A descriptor set layout acts as a blueprint, telling the pipeline how many descriptors will be bound, and what types of resources each descriptor uses.

In this tutorial, we create abstractions for descriptor sets, descriptor set layouts and descriptor pools, to make these objects easier to work with within the engine. We create a descriptor set layout that contains a single buffer descriptor, and use this to bind the uboBuffer for use within the vertex shader.

[View Tutorial Changes](https://github.com/blurrypiano/littleVulkanEngine/commit/17b43cb62389e75d9763ccfed503702281e76473)

[Video Tutorial](https://youtu.be/d5p44idnZLQ)

[Descriptors Header](https://pastebin.com/yU7dMAxt)

[Descriptors Implementation](https://pastebin.com/hZ6ax53w)

## Further Reading

[VkGuide.dev - Abstracting Descriptors](https://vkguide.dev/docs/extra-chapter/abstracting_descriptors/)

[Vulkan Tutorial - Descriptor Sets](https://vulkan-tutorial.com/Uniform_buffers/Descriptor_layout_and_buffer)

## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations.

Build the project using make, and run. The project should compile successfully and displaytwo vases. Move the camera with the arrow and WASD keys.

## Fixing Common Issues

The most likely cause for an error is that the relative filepaths are not correct. This can happen if the current working directory for when the program executable is run is not the same as the project directory. In this case try changing "shaders/simple_shader.vert.spv" as "shaders/simple_shader.frag.spv" in simple_render_system.cpp to use an absolute path instead, as well the filepath for "models/smooth_vase.obj" in first_app.cpp
