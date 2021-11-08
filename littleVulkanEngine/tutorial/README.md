# Upcoming Tutorial Drafts

I'm planning to cover this content over the course of the next few tutorials. This is a currently just a draft, so the real tutorials will be a cleaned up and modified version of this.

## Building

Create a directory for your project and copy the contents of tutorial to your directory. Rename .env-example to .env and enter the path to where you installed the vulkan sdk.

If you are not using the makefile provided, make sure to compile your shader code to the .spv files (see tutorial 2).

Build the project using make, and run. You should see a two vases. Move the camera with WASD and the arrow keys.

## Errors

The most likely cause for an error is that the relative filepaths are not correct. This can happen if the current working directory for when the program executable is run is not the same as the project directory. In this case try changing "shaders/simple_shader.vert.spv" as "shaders/simple_shader.frag.spv" in simple_render_system.cpp to use an absolute path instead, as well the filepath for the vases objects "models/smooth_vase.obj" in first_app.cpp
