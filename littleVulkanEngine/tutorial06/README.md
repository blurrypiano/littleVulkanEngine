# Tutorial

In previous tutorials, we’ve hardcoded vertex data directly into our vertex shader. However, this is clearly not a feasible solution for drawing anything remotely complicated. 3D models for modern games typically have thousands of triangles.


So in this tutorial, we will cover how vertex buffers store data, and can be bound to graphics pipelines.


A vertex buffer is really just a chunk of memory that we pass into our vertex shader. We can put whatever data we would like into this memory as long as we tell our graphics pipeline how it is structured. 


## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations

Build the project using make, and run. The project should compile successfully and when run display a yellow triangle on a black background