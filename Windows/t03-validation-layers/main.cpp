#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

class HelloTriangleApplication
{
public:
	void run()
	{
		initialize_window();
		initialize_vulkan();
		main_loop();
		clean_up();
	}

private:
	VkInstance m_vulkan_instance;
	GLFWwindow* m_window;
	const uint32_t
		m_width = 800,
		m_height = 600;
	const std::vector<const char*> m_validation_layers = {
		"VK_LAYER_KHRONOS_validation"
	};

#ifdef NDEBUG
	const bool m_enable_validation_layers = false;
#else
	const bool m_enable_validation_layers = true;
#endif

	void initialize_window()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);	// do not create OpenGl context
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);		// disable window resizing
		m_window = glfwCreateWindow(m_width, m_height, "Vulkan Tutorial", nullptr, nullptr);
	}

	void initialize_vulkan()
	{
		create_vulkan_instance();
	}

	void create_vulkan_instance()
	{
		if (m_enable_validation_layers and not check_validation_layer_support())
		{
			throw std::runtime_error("Validation layers requested, but not available!");
		}

		VkApplicationInfo application_info{};
		application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		application_info.pApplicationName = "Hello Triangle";
		application_info.apiVersion = VK_MAKE_VERSION(1, 0, 0);
		application_info.pEngineName = "No Engine";
		application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		application_info.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo instance_info{};
		instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_info.pApplicationInfo = &application_info;

		uint32_t glfw_extension_count = 0;
		const char** glfw_extensions;
		glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

		instance_info.enabledExtensionCount = glfw_extension_count;
		instance_info.ppEnabledExtensionNames = glfw_extensions;
		if (m_enable_validation_layers)
		{
			instance_info.enabledLayerCount = static_cast<uint32_t>(m_validation_layers.size());
			instance_info.ppEnabledLayerNames = m_validation_layers.data();
		}
		else
		{
			instance_info.enabledLayerCount = 0;
		}

		if (vkCreateInstance(&instance_info, nullptr, &m_vulkan_instance) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan instance!");
		}
	}

	bool check_validation_layer_support()
	{
		uint32_t layer_count;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr); // get count for vector size
		std::vector<VkLayerProperties> available_layers { layer_count };
		vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

		for (const auto& layer_name : m_validation_layers)
		{
			bool layer_found = false;
			for (const auto& layer_properties : available_layers)
			{
				if (strcmp(layer_name, layer_properties.layerName) == 0)
				{
					layer_found = true;
					break;
				}
			}

			if (not layer_found)
			{
				return false;
			}
		}
		return true;
	}

	void main_loop()
	{
		while (not glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();
		}
	}

	void clean_up()
	{
		vkDestroyInstance(m_vulkan_instance, nullptr);
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
};

int main()
{
	std::cout << "Tutorial 03: Validation Layers\n";
	HelloTriangleApplication application{};

	try
	{
		application.run();
	}
	catch (std::exception& e)
	{	
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}