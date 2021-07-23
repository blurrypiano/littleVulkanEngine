# Tutorial 16

Index buffers are a way to reduce the amount of gpu memory required to store a model’s attribute data by allowing duplicate vertex data to be removed from the Vertex Buffer. An Index Buffer acts as an array of pointers into the Vertex Buffer, allowing vertex data to be reused by multiple triangles.

A staging buffer is used in the process of transferring data from the host (cpu) to the device (gpu), and is a temporary location to hold data so that it can then be transferred to more optimal (faster) memory. 

In this tutorial we update the model class to support index buffers, and use staging buffers to transfer the model data to device optimal memory. 

[View Tutorial Changes](https://github.com/blurrypiano/littleVulkanEngine/commit/411336151b35c9434b0df73fd4f0f94febd99589) 

[Video Tutorial](https://youtu.be/qxuvQVtehII)

[Paste Bin - Updated createCubeModel()](https://pastebin.com/4T10MFgb)

[PolyHaven - Apple 3D Model](https://polyhaven.com/a/food_apple_01)

## Resources

[Vulkan-Tutorial Index Buffers](https://vulkan-tutorial.com/Vertex_buffers/Index_buffer)

[Vulkan-Tutorial Staging Buffers](https://vulkan-tutorial.com/Vertex_buffers/Staging_buffer)

## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations.

Build the project using make, and run. The project should compile successfully and display a multi-colored cube. Move the camera with the arrow and WASD keys.
