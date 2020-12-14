# Tutorial

In this tutorial we continue working on our graphics pipeline. We read in the compiled shader files created in the previous tutorial and use them to create shader modules.


We also have added the lve_device file which encapsulates a vulkan device and functionality for creating a vulkan instance, setting up validation layers, picking a physical device and creating a logical device and queues. Additionally, there is a bit of helper functionality


## Building

Follow the steps in tutorial 0 that go over how to set up your development environment. Next create a directory for your project and copy the contents of this tutorial into your directory. Rename .env-example to .env and enter the path to where you installed the vulkan sdk.

Build the project using make, and run. The project should compile successfully but when run produce an assertation failure that pipeline layout was not provided in config info. This will be fixed in tutorial 04
