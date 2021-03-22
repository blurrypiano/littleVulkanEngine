# Tutorial

In this tutorial we set up a callback function that listens to changes on the glfw window and updates the dimensions of the window wrapper class. When the window is resized we must create a new compatible swap chain.

We change to using a dynamic viewport and scissor so that graphics pipeline creation is no longer dependent on the swap chains dimensions.

** [View Tutorial Changes](https://github.com/blurrypiano/littleVulkanEngine/commit/ac911abade3f0747c31b5cf3d21c59c7c077d6f4) **

** [Video Tutorial](https://youtu.be/0IIqvi3Z0ng) **


## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations

Build the project using make, and run. The project should compile successfully and when display a rainbow triangle on a gray background
