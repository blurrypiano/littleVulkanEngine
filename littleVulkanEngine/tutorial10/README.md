# Tutorial 10

Matrices are an incredibly useful tool for representing linear transformations on geometry. In this tutorial we cover 2d transformation matrices and use them in our shaders to stretch, shrink and rotate our vertex position.

I also introduce the idea of a runtime game object model, and we create a (very) simple game object class to manage the different entities and components they are made up of. 

[View Tutorial Changes](https://github.com/blurrypiano/littleVulkanEngine/commit/c4d0ca7ac67c33b98c8dd69e854276e5e8281b6a) 

[Video Tutorial](https://youtu.be/gxUcgc88tD4)

## Resources

[Scatchapixel's Maths for computer graphics](https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/points-vectors-and-normals)

[3Blue1Brown Linear algrebra playlist](https://www.youtube.com/playlist?list=PLZHQObOWTQDPD3MizzM2xVFitgF8hE_ab) 

[Column Major Order explained](https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/row-major-vs-column-major-vector) 

[Object oriented Game Engine Architecture](https://homepages.fhv.at/thjo/lecturenotes/sysarch/game-engine-architecture.html#game-objects) 

[Ray Renderlich Introduction to Entity Component Systems (good explanation)](https://www.raywenderlich.com/2806-introduction-to-component-based-architecture-in-games)

[A c++ ECS implementation I like by Austin Morlan](https://austinmorlan.com/posts/entity_component_system/)

## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations.

Build the project using make, and run. The project should compile successfully and when display a rotating green triangle on the screen.
