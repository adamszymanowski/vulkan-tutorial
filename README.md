# Vulkan Tutorial
Me doing [Vulkan Tutorial](https://vulkan-tutorial.com/)

- [tutorial 00 (development environment)](https://vulkan-tutorial.com/Development_environment)

## Setup
### Windows
Visual Studio 2019
- Project -> Properties -> C/C++ -> General -> Additional Include Directories
  * `C:\glfw-3.3.8.bin.WIN64\include`
  * `C:\VulkanSDK\1.3.224.0\Include`
- Project -> Properties -> Linker -> General -> Additional Library Directories
  * `C:\glfw-3.3.8.bin.WIN64\lib-vc2019`
  * `C:\VulkanSDK\1.3.224.0\Lib`
- Project -> Properties -> Linker -> Input -> Additional Dependencies
  * `vulkan-1.lib`
  * `glfw3.lib`