# Tutorial 11

This is a coding focused tutorial with not much theory. We restructure our application class by creating a Renderer class and a SimpleRenderSystem class. This modular and flexible design should make implementing many future topics possible and easy to integrate into the engine. 

• The renderer is responsible for managing the swap chain, command buffers, and drawing functionality
• The SimpleRenderSystem manages a pipeline and its layout, and provides the data necessary to render a list of game objects

We also add a check to ensure recreated swap chain render passes remain compatible, and make the renderer class’ command buffer resources independent of the swap chain’s image count.

[View Tutorial Changes](https://github.com/blurrypiano/littleVulkanEngine/commit/0e92b3678b637258e4bfbc59ef253f486c814aaf) 

[Video Tutorial](https://youtu.be/uGRSTRGlZVs)

## Resources

[RainbowSystem example](https://pastebin.com/si4eaa3g)

[Gravity & vector field system example](https://pastebin.com/7NCfdNrd)
See physicsDemo.patch file for exact changes

## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations.

Build the project using make, and run. The project should compile successfully and when display a rotating green triangle on the screen.
