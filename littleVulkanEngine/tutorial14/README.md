# Tutorial 14

We implement the camera/view transform. This allows us to place a virtual camera anywhere within the game world, which makes it possible to control what is captured by the viewing frustum and displayed to the screen. 

[View Tutorial Changes](https://github.com/blurrypiano/littleVulkanEngine/commit/edbec223bcf8012c92aca46da5f090fbb1ba86f3) 

[Video Tutorial](https://youtu.be/rvJHkYnAR3w)

[Set View Matrices paste bin](https://pastebin.com/raw/cvCeZcz4)

## Resources

Fundamentals of Computer Graphics by Peter Shirley & Steve Marschner

## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations.

Build the project using make, and run. The project should compile successfully and display a rotating multi-colored cube.