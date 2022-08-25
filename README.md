# Vulkan Tutorial
Me doing [Vulkan Tutorial](https://vulkan-tutorial.com/)

- [tutorial 00 (development environment)](https://vulkan-tutorial.com/Development_environment)

## references
- [glfw Window](https://www.glfw.org/docs/3.3/window_guide.html#window_object)

## Setup
### Windows
Install Vulkan SDK [https://sdk.lunarg.com/sdk/download/1.3.224.0/windows/VulkanSDK-1.3.224.0-Installer.exe](https://sdk.lunarg.com/sdk/download/1.3.224.0/windows/VulkanSDK-1.3.224.0-Installer.exe) to:
- `C:\VulkanSDK\1.3.224.0`

Unpack precompiled GLFW [https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.bin.WIN64.zip](https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.bin.WIN64.zip) to:
- `C:\glfw-3.3.8.bin.WIN64`

**Visual Studio 2019**
- Project -> Properties -> C/C++ -> General -> Additional Include Directories
  * `C:\glfw-3.3.8.bin.WIN64\include`
  * `C:\VulkanSDK\1.3.224.0\Include`
- Project -> Properties -> Linker -> General -> Additional Library Directories
  * `C:\glfw-3.3.8.bin.WIN64\lib-vc2019`
  * `C:\VulkanSDK\1.3.224.0\Lib`
- Project -> Properties -> Linker -> Input -> Additional Dependencies
  * `vulkan-1.lib`
  * `glfw3.lib`