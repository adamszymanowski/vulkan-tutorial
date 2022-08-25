#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#include <iostream>
#include <stdexcept>
#include <cstdlib>

class HelloTriangleApplication
{
public:
	void run()
	{
		init_window();
		init_vulkan();
		main_loop();
		cleanup();
	}

private:
	void init_window()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // do not create OpenGL context
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // disable window resizing
		m_window = glfwCreateWindow(m_width, m_height, "Vulkan Tutorial", nullptr, nullptr);
 	}

	void init_vulkan()
	{

	}

	void main_loop()
 	{
		while (not glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();
		}
	}

	void cleanup()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	const uint32_t m_width = 800;
	const uint32_t m_height = 600;
	GLFWwindow* m_window;
};

int main()
{
	std::cout << "Tutorial 01: Base code\n";
	HelloTriangleApplication app;

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}