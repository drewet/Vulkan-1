VulKan ToolS (VKTS):
--------------------

Authors:

**Norbert Nopper** (norbert@nopper.tv)

VKTS version: **0.9.0**

Depending Vulkan API header version: **1.0.3.1**

Depending Vulkan extensions, when using a window system:

- `VK_KHR_swapchain`
- `VK_KHR_surface`

- Linux X11: `VK_KHR_xlib_surface` or
             `VK_KHR_xcb_surface`
- Android:   `VK_KHR_android_surface`
- Windows:   `VK_KHR_win32_surface`


About:
------

VulKan ToolS (VKTS) is a cross platform, C++ 11 helper library for [Vulkan](https://www.khronos.org/vulkan) released under the [MIT License](VKTS_license.txt).


Used libraries (beside C++ 11):
-------------------------------

- [LunarG Vulkan SDK](http://vulkan.lunarg.com)
- [GLM](http://glm.g-truc.net)
- [FreeType](http://www.freetype.org)


Supported platforms:
--------------------

- Linux X11 or console only (GNU GCC)
- Android (GNU GCC) 
- Microsoft Windows or console only (GNU MinGW & Visual C++ 2015)


Features:
---------

- Platform abstraction.
- Logging functions.
- Matrix and quaternion classes and functions.
- Vulkan wrapper objects and factory functions.
- Basic multi threaded engine including synchronization, time and CPU querying.   
- Basic native display handling including multi display support.
- Basic native window handling including fullscreen support.
- Basic input handling: Gamepad, keyboard and mouse.
- None visual configuration for console/compute applications.
- Loading and saving of text, binary and image files (TGA and HDR).
- VKTS scene file format:
    - 3D objects (incl. node hierarchy)
    - Meshes (incl. sub meshes)
    - Materials (Phong and BRDF)
    - Textures (TGA and HDR)
    - Animations (incl. node and armature)
- Blender Python exporter for the VKTS scene file format.
- GLSL to SPIR-V build script.
- Interpolator and converter for constant, linear and bezier key frame data.
- FreeType font support.


How to build:
-------------

- [System requirements and step by step tutorial](/VKTS_Documentation/BUILD.md)


Changelog:
----------

02/16/2016 - First public release.