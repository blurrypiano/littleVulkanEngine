# Intro to Point Lights - Tutorial 21

Point lights are infinitesimally small sources of light. They differ from directional lighting in two major ways: the direction to the light source will be different for each vertex and the intensity of light is attenuated based on the distance between the vertex and the light’s position.

In this tutorial we add a point light object to the global UBO and update the vertex shader to make use of this new lighting technique.

[quad.obj](https://pastebin.com/A8Q7tdeJ)

[View Tutorial Changes](https://github.com/blurrypiano/littleVulkanEngine/commit/89170199fd547857687c669be39de23e2f666b51)

[Video Tutorial](https://youtu.be/Z1lLwAEMt4M)

## Further Reading and Video Resources

[Wikipedia - Inverse Square Law](https://en.wikipedia.org/wiki/Inverse-square_law)

[Parameterized Point Light Attenuation](https://www6.uniovi.es/java3d/slides/mt0362.htm)

[StackOverflow - Should you use vec3 in UBOs](https://stackoverflow.com/questions/38172696/)

[O'Reilly std140 Layout Rules](https://www.oreilly.com/library/view/opengl-programming-guide/9780132748445/app09lev1sec2.html)

## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations.

Build the project using make, and run. The project should compile successfully and display two vases. Move the camera with the arrow and WASD keys.

## Fixing Common Issues

The most likely cause for an error is that the relative filepaths are not correct. This can happen if the current working directory for when the program executable is run is not the same as the project directory. In this case try changing "shaders/simple_shader.vert.spv" as "shaders/simple_shader.frag.spv" in simple_render_system.cpp to use an absolute path instead, as well the filepath for the model objects "models/*.obj" in first_app.cpp
