# Diffuse Shading - Tutorial 18

In this tutorial we implement a diffuse lighting model with a constant ambient light term in the vertex shader. This colors the vase object by calculating the intensity of light for each vertex based on how directly the surface at the point faces the incoming light.

We use a directional light source, which means the same direction is used for every vertex. This simulates a light source that is infinitely far away, with all incoming light rays being parallel. A real world example of this would be sunlight, since due to its extreme distance, incoming light rays can be treated as parallel.

[View Tutorial Changes](https://github.com/blurrypiano/littleVulkanEngine/commit/8af749ded9c280da08a05a5f06f8df477c4e17b6) 

[Video Tutorial](https://youtu.be/wfh2N4u-nOU)

## Resources

[Physically Based Rendering - The light transport equation](https://www.pbr-book.org/3ed-2018/Light_Transport_I_Surface_Reflection/The_Light_Transport_Equation)

[Scratchapixel - Diffuse and lambertian shading](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/diffuse-lambertian-shading)

[Learning Modern 3D Graphics Programming - Transforming Normals](https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html)

[Learning Modern 3D Graphics Programming - Ambient Lighting](https://paroj.github.io/gltut/Illumination/Tut09%20Global%20Illumination.html)

[Properties of inverse matrices](https://www.web-formulas.com/Math_Formulas/Linear_Algebra_Properties_of_Inverse_Matrices.aspx)


## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations.

Build the project using make, and run. The project should compile successfully and displaytwo vases. Move the camera with the arrow and WASD keys.

## Fixing Common Issues

The most likely cause for an error is that the relative filepaths are not correct. This can happen if the current working directory for when the program executable is run is not the same as the project directory. In this case try changing "shaders/simple_shader.vert.spv" as "shaders/simple_shader.frag.spv" in simple_render_system.cpp to use an absolute path instead, as well the filepath for "models/smooth_vase.obj" in first_app.cpp


