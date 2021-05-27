# Tutorial 12

We finally moved to 3D! In this tutorial we update the engine to use 3D positions and homogeneous coordinates, opening the possibility to more complicated transformation matrices. 

[View Tutorial Changes](https://github.com/blurrypiano/littleVulkanEngine/commit/6cc8edabc8aa1dd54bc1faa76884ee1e348442d7) 

[Video Tutorial](https://youtu.be/0X_kRtyVzm4)


## Resources

[Wikipedia Euler Angles](https://en.wikipedia.org/wiki/Euler_angles)

[Create Cube Model](https://pastebin.com/iZSg0XHh)

[Fast mat4() implementation](https://pastebin.com/KFvG09A8)

[Understanding Quaternions](https://www.3dgep.com/understanding-quaternions/)

## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations.

Build the project using make, and run. The project should compile successfully and when display a rotating green triangle on the screen.
