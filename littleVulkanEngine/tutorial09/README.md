# Tutorial 09

 Vulkan Push constants are an easy and performant way to pass data to a shader, via a vulkan command buffer rather than writes to memory or copy commands. They’re a great option for data that updates frequently, however have a main drawback of being limited in size.

In this tutorial I provide an introduction to vulkan push constants and show a simple method for easily drawing multiple copies of an object as well as applying motion. 

[View Tutorial Changes](https://github.com/blurrypiano/littleVulkanEngine/commit/eb41544f3bf26130ca4d3872bbdc84a484171dc4) 

[Video Tutorial](https://youtu.be/wlLGLWI9Fdc)

## Resources

[3Blue1Brown Linear algrebra playlist](https://www.youtube.com/playlist?list=PLZHQObOWTQDPD3MizzM2xVFitgF8hE_ab) 

[VkGuide.dev Push Constants](https://vkguide.dev/docs/chapter-3/push_constants/) 

[SaschaWillems Push Constants](https://github.com/SaschaWillems/Vulkan/tree/master/examples/pushconstants) 

[Vulkan Spec - Push Constant Updates](https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/chap14.html#_push_constant_updates) 

[Vulkan Spec - Push Constant Interface](https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/chap15.html#interfaces-resources-pushconst) 

[Vulkan Spec - Offset and Stride alignment](https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/chap15.html#interfaces-resources-layout) 

[Vulkan tutorial - Alignment Requirements](https://vulkan-tutorial.com/Uniform_buffers/Descriptor_pool_and_sets#page_Alignment-requirements) 


## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations

Build the project using make, and run. The project should compile successfully and when display a stack of blue triangles moving across the screen
