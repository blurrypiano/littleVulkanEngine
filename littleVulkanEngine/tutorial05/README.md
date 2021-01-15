# Tutorial

We finally draw a triangle!! In this tutorial we cover command buffers and give a brief overview of the swap chain.

Command buffers are one of the reasons for improved performance over previous APIs such as openGL. Instead of submitting draw commands every frame, we can record our draw commands to a command buffer, and submit the entire buffer. This minimizes the overhead accumulated by having to submit each command seperately. Buffer objects can also be re-used when draw commands are not changing between frames, as well as makes multi-thread recording of draw calls possible.

The LveSwapChain file is a helper file to hide some functionality of the vulkan API away from us for now. I believe that the topics of synchronization, framebuffer creation, render passes, triple buffering etc, are unnecessary to cover in detail at this early of a stage when being introduced to the vulkan API. When we start covering more advanced topics, such as reflections and shadow mapping, is when we'll start to take a closer look. However, for those of you who want a deeper understanding now, this code very closely corresponds to the *Swap Chain*, *Image View*,  *Framebuffer* and *Rendering and Presentation* sections from [Vulkan Tutorial](https://vulkan-tutorial.com/Drawing_a_triangle/Presentation/Swap_chain).

[Double vs Triple Buffering (Great explanation!)](https://gamedev.stackexchange.com/questions/82318/what-problem-does-double-or-triple-buffering-solve-in-modern-games)

More on [present modes & the swap chain](https://developer.samsung.com/sdp/blog/en-us/2019/07/26/vulkan-mobile-best-practice-how-to-configure-your-vulkan-swapchain)

## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and enter the path to where you installed the vulkan sdk.

Build the project using make, and run. The project should compile successfully and when run display a red triangle centered in the window.
