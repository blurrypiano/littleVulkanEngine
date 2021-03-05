# Tutorial

In this tutorial we change our command buffers to be re-recorded before each frame. We change our window class to support resizing and will rebuild the swap chain (and objects that depend on the swap chain such as our graphics pipeline).

## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations

Build the project using make, and run. The project should compile successfully and when display a rainbow triangle on a gray background