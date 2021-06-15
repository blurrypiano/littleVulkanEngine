# Tutorial 13

We implement functions to create the orthographic projection and the perspective projection matrices. Projection matrices change the shape and location of vulkan’s canonical view volume. This allows us to render objects outside of the canonical vulkan view volume, and apply perspective to the objects, making objects appear smaller as their z-depth increases.

[View Tutorial Changes](https://github.com/blurrypiano/littleVulkanEngine/commit/011498dbbbfc303293943ca51934686ba66ebbdd) 

[Theory Video - Perspective Projection Matrices](https://youtu.be/U0_ONQQ5ZNM)

[Video Tutorial](https://youtu.be/0X_kRtyVzm4)

[Projection Matrices paste bin](https://pastebin.com/eFwnyuX6)

## Resources

Fundamentals of Computer Graphics by Peter Shirley & Steve Marschner

[A projection matrix derivation for openGL](http://www.songho.ca/opengl/gl_projectionmatrix.html)

[Blog post on vulkans coordinate system](https://matthewwellings.com/blog/the-new-vulkan-coordinate-system/)


## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations.

Build the project using make, and run. The project should compile successfully and display a rotating multi-colored cube.