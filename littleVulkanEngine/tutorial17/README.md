# Tutorial 17

In this tutorial we make use of tinyobjloader, a tiny but powerful single file wavefront obj loader, to load 3D models into the engine.

[TinyObjLoader Header File](https://github.com/tinyobjloader/tinyobjloader/blob/master/tiny_obj_loader.h)

[Models Sharedrive](https://drive.google.com/drive/folders/1QuvSRG4HCxCfC5k0F5G03tNwUQoqE8f3?usp=sharing)

[Paste Bin - hashCombine](https://pastebin.com/4T10MFgb)

[View Tutorial Changes](https://github.com/blurrypiano/littleVulkanEngine/commit/b1b665ed9eb82c69f99e56fe42e82d081a4a9203) 

[Video Tutorial](https://youtu.be/jdiPVfIHmEA)

## Resources

[Vulkan-Tutorial Loading Models](https://vulkan-tutorial.com/Loading_models)

[Wikipedia - Wavefront File object format](https://en.wikipedia.org/wiki/Wavefront_.obj_file)

[Tiny object loader git repo](https://github.com/tinyobjloader/tinyobjloader)

[Hash Combine explanation](https://stackoverflow.com/a/57595105)

[Cpp reference - Fold expressions](https://en.cppreference.com/w/cpp/language/fold)

[PolyHaven - Ceramic Vase Model](https://polyhaven.com/a/ceramic_vase_01)

[Importing .obj colors](https://blender.stackexchange.com/questions/31997/how-can-i-get-vertex-painted-obj-files-to-import-into-blender)

## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and update the filepaths to your installed locations.

Download the [models directory](https://drive.google.com/drive/folders/1QuvSRG4HCxCfC5k0F5G03tNwUQoqE8f3?usp=sharing) an add to your project (as demonstrated in tutorial 17).

Build the project using make, and run. The project should compile successfully and display a multi-colored cube. Move the camera with the arrow and WASD keys.

## Fixing Common Issues

The most likely cause for an error is that the relative filepaths are not correct. This can happen if the current working directory for when the program executable is run is not the same as the project directory. In this case try changing "shaders/simple_shader.vert.spv" as "shaders/simple_shader.frag.spv" in simple_render_system.cpp to use an absolute path instead, as well the filepath for "models/smooth_vase.obj" in first_app.cpp


