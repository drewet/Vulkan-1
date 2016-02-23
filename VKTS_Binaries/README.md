VulKan ToolS (VKTS) Binaries:
-----------------------------

This folder and sub folders contains all executables, shaders and assets. The examples are copied  
into this folder after the build. This folder has to be the working directory, otherwise shaders  
and assets are not found.

Using Android, the asset files have to be loacted in `/sdcard/Download/VKTS_Binaries` on the device.
The scripts `android_copy.bat` and `android_copy.sh` do create the directory and copies all the files from the host system to the device.
  

Inside `shader/GLSL/` there are two build scripts, which do compile all available GLSL source files.  
The resulting SPIR-V binaries are deployed into `shader/SPIR/V`.  

To enable all debug layers (without VK_LAYER_LUNARG_api_dump), set the following debug environment variables:

Linux:
export VK_INSTANCE_LAYERS=VK_LAYER_LUNARG_device_limits:VK_LAYER_LUNARG_draw_state:VK_LAYER_LUNARG_image:VK_LAYER_LUNARG_mem_tracker:VK_LAYER_LUNARG_object_tracker:VK_LAYER_LUNARG_param_checker:VK_LAYER_LUNARG_swapchain:VK_LAYER_LUNARG_threading
export VK_DEVICE_LAYERS=VK_LAYER_LUNARG_device_limits:VK_LAYER_LUNARG_draw_state:VK_LAYER_LUNARG_image:VK_LAYER_LUNARG_mem_tracker:VK_LAYER_LUNARG_object_tracker:VK_LAYER_LUNARG_param_checker:VK_LAYER_LUNARG_swapchain:VK_LAYER_LUNARG_threading

Windows:
set VK_INSTANCE_LAYERS=VK_LAYER_LUNARG_device_limits;VK_LAYER_LUNARG_draw_state;VK_LAYER_LUNARG_image;VK_LAYER_LUNARG_mem_tracker;VK_LAYER_LUNARG_object_tracker;VK_LAYER_LUNARG_param_checker;VK_LAYER_LUNARG_swapchain;VK_LAYER_LUNARG_threading
set VK_DEVICE_LAYERS=VK_LAYER_LUNARG_device_limits;VK_LAYER_LUNARG_draw_state;VK_LAYER_LUNARG_image;VK_LAYER_LUNARG_mem_tracker;VK_LAYER_LUNARG_object_tracker;VK_LAYER_LUNARG_param_checker;VK_LAYER_LUNARG_swapchain;VK_LAYER_LUNARG_threading


Used libraries:
---------------

- [LunarG Vulkan SDK](http://vulkan.lunarg.com) (see [License](/VKTS/LunarG_license.html))