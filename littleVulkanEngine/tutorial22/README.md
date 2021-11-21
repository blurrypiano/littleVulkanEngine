# Vertex vs Fragment Lighting - Tutorial 22

Currently all lighting calculations are being performed within a vertex shader, meaning that light intensity is only calculated for each vertex, and the computed color is then blended across the fragments of each triangle. Rather than interpolating the final color value, we can instead interpolate the surface normal at each vertex. Then within the fragment shader we use the interpolated normal vector, and the direction to the light source from the position on the fragment, to calculate the intensity of light at each fragment. 

[View Tutorial Changes](https://github.com/blurrypiano/littleVulkanEngine/commit/26db088c1daa7cf585040bd1280a1d8eebba2986)

[Video Tutorial](https://youtu.be/YnMyKHfrgU4)

## Further Reading and Video Resources

[Fragment Lighting](https://paroj.github.io/gltut/Illumination/Tut10%20Fragment%20Lighting.html)

## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations.

Build the project using make, and run. The project should compile successfully and display two vases. Move the camera with the arrow and WASD keys.

## Fixing Common Issues

The most likely cause for an error is that the relative filepaths are not correct. This can happen if the current working directory for when the program executable is run is not the same as the project directory. In this case try changing "shaders/simple_shader.vert.spv" as "shaders/simple_shader.frag.spv" in simple_render_system.cpp to use an absolute path instead, as well the filepath for the model objects "models/*.obj" in first_app.cpp
