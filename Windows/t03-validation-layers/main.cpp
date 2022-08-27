#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

VkResult create_vulkan_debug_messenger(
	VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* debug_info,
	const VkAllocationCallbacks* allocator,
	VkDebugUtilsMessengerEXT* debug_messenger)
{
	auto create = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, 
																			"vkCreateDebugUtilsMessengerEXT");
	if (create != nullptr)
	{
		return create(instance, debug_info, allocator, debug_messenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void destroy_vulkan_debug_messenger(
	VkInstance instance, 
	VkDebugUtilsMessengerEXT debug_messenger, 
	const VkAllocationCallbacks* allocator)
{
	auto destroy = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, 
																			  "vkDestroyDebugUtilsMessengerEXT");
	if (destroy != nullptr)
	{
		destroy(instance, debug_messenger, allocator);
	}

}

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
	VkInstance m_vulkan_instance{0};
	GLFWwindow* m_window{0};
	VkDebugUtilsMessengerEXT m_debug_messenger{ 0 };
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

	void main_loop()
	{
		while (not glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();
		}
	}

	void clean_up()
	{
		if (m_enable_validation_layers)
		{
			destroy_vulkan_debug_messenger(m_vulkan_instance, m_debug_messenger, nullptr);
		}
		vkDestroyInstance(m_vulkan_instance, nullptr);
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	// Vulkan-specific helpers
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

		auto extensions = get_reqired_extensions();
		instance_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		instance_info.ppEnabledExtensionNames = extensions.data();
		
		VkDebugUtilsMessengerCreateInfoEXT debug_info{};

		if (m_enable_validation_layers)
		{
			instance_info.enabledLayerCount = static_cast<uint32_t>(m_validation_layers.size());
			instance_info.ppEnabledLayerNames = m_validation_layers.data();

			populate_debug_messenger(debug_info);
			instance_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) & debug_info;
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

	void setup_debug_messenger()
	{
		if (not m_enable_validation_layers) return;

		VkDebugUtilsMessengerCreateInfoEXT debug_info;
		populate_debug_messenger(debug_info);

		if (create_vulkan_debug_messenger(m_vulkan_instance, &debug_info, nullptr, &m_debug_messenger) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to setup Vulkan debug messenger!");
		}
	}

	void populate_debug_messenger(VkDebugUtilsMessengerCreateInfoEXT& debug_info)
	{
		debug_info = {};
		debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debug_info.messageSeverity = (
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
		);
		debug_info.messageType = (
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
		);
		debug_info.pfnUserCallback = debug_callback;
	}

	std::vector<const char*> get_reqired_extensions()
	{
		uint32_t glfw_extension_count = 0;
		const char** glfw_extensions;
		glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

		std::vector<const char*> extensions{ glfw_extensions, glfw_extensions + glfw_extension_count };
		if (m_enable_validation_layers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
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

	static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
		VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
		VkDebugUtilsMessageTypeFlagsEXT message_type,
		const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
		void* user_data)
	{
		std::cerr << "validation layer: " << callback_data->pMessage << std::endl;
		return VK_FALSE;
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