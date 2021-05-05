# Imgui Demo

Demo of how imgui can be integrated into the project. This demo uses the tutorial 11 code as the starting point. I'm not going to
create a video tutorial covering this topic yet, so I've included lots of comments on the code instead. But lots of people were
asking how imgui could be added so I figure this will be helpful to some people.

Also note: I haven't connected things in the example such as setting the clear color, to actually update the renderers clear color. But this
should be pretty straightforward to do.

## Building

Create a directory for your project and copy the contents of tutorial to your directory. Rename .env-example to .env and enter the path to where you installed the vulkan sdk, as well as the imgui library.

At this point this current build system is definitely starting to show its cracks, since everything is rebuilt every compile, which now also includes the entire imgui lib.

Build the project using make, and run. You should a rotating green triangle, as well as some imgui demo windows.
