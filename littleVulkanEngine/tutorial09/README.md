# Tutorial

 Vulkan Push constants are an easy and performant way to pass data to a shader, via a vulkan command buffer rather than writes to memory or copy commands. They’re a great option for data that updates frequently, however have a main drawback of being limited in size.

In this tutorial I provide an introduction to vulkan push constants and show a simple method for easily drawing multiple copies of an object as well as applying motion. 


## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations

Build the project using make, and run. The project should compile successfully and when display a stack of blue triangles moving across the screen
