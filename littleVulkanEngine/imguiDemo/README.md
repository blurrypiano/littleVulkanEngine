# Imgui Demo

Demo of how imgui can be integrated into the project. This demo uses the tutorial 11 code as the starting point. I'm not going to
create a video tutorial covering this topic yet, so I've included lots of comments on the code instead. But lots of people were
asking how imgui could be added so I figure this will be helpful to some people.

Also note: I haven't connected things in the example such as setting the clear color, to actually update the renderers clear color. But this
should be pretty straightforward to do.

# Integration steps

1. Copy the core (imgui files)[https://github.com/ocornut/imgui] somewhere into your project. In my case I've put them into an external/imgui subdirectory. Then from the backends directory you will also need to copy imgui_impl_glfw.hpp, imgui_impl_glfw.cpp, imgui_impl_vulkan.hpp, and imgui_impl_vulkan.cpp

2. Check to see if any changes/updates for your build system, are required. For me this required updating my cflags to include the path to my imgui subfolder and udpating my makefile to build the imgui cpp files with the rest of the engine.

3. Integrate imgui into your code. Here's my  (change file for example)[https://github.com/blurrypiano/littleVulkanEngine/commit/c25099a7da6770072fdfc9ec3bd4d38aa9380906].


## Building

Create a directory for your project and copy the contents of tutorial to your directory. Rename .env-example to .env and enter the path to where you installed the vulkan sdk, as well as the imgui library.

At this point this current build system is definitely starting to show its cracks, since everything is rebuilt every compile, which now also includes the entire imgui lib.

Build the project using make, and run. You should a rotating green triangle, as well as some imgui demo windows.
