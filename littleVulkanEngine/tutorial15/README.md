# Tutorial 15

Getting user input and time loops are both integral parts to any game engine and are complex topics. In this tutorial I briefly introduce the concepts and we add the ability to dynamically move the camera in a frame rate independent way. 

I highly recommend these two resources for further reading on the game loop programming pattern as they go into more depth than I go into here, and bring up some common issues that may not be immediately noticeable with the basic implementation I demonstrate in this video.

[Programming Patterns - Game Loop](https://gameprogrammingpatterns.com/game-loop.html)
[Fix your timestep](https://gafferongames.com/post/fix_your_timestep/)

For user input the tutorial series will focus on GLFW because I’d like to avoid the additional complexity while we cover other topics. After we’ve covered some more vulkan material, we will return to these topics in greater detail.

[A GLFW Keyboard and mouse tutorial](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/)
[GLFW Input Guide](https://www.glfw.org/docs/3.3/input_guide.html)

[View Tutorial Changes](https://github.com/blurrypiano/littleVulkanEngine/commit/edbec223bcf8012c92aca46da5f090fbb1ba86f3) 

[Video Tutorial](https://youtu.be/wFV9zPU_Cjg)

[KeyMappings Struct paste bin](https://pastebin.com/raw/V1539Dz5)

## Resources

Fundamentals of Computer Graphics by Peter Shirley & Steve Marschner
[Programming Patterns - Game Loop](https://gameprogrammingpatterns.com/game-loop.html)
[Fix your timestep](https://gafferongames.com/post/fix_your_timestep/)
[A GLFW Keyboard and mouse tutorial](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/)
[GLFW Input Guide](https://www.glfw.org/docs/3.3/input_guide.html)

## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations.

Build the project using make, and run. The project should compile successfully and display a multi-colored cube. Move the camera with the arrow and WASD keys.