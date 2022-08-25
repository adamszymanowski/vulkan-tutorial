#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

class HelloTriangleAplication
{
public:
	void run()
	{
		initialize_window();
		main_loop();
		clean_up();
	}
private:
	void initialize_window()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // do not create OpenGl context
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // disable window resizing
		m_window = glfwCreateWindow(m_width, m_height, "Vulkan Tutorial", nullptr, nullptr);
	}

	void initialize_vulkan()
	{

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
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	const uint32_t
	m_width = 800,
	m_height = 600;

	GLFWwindow* m_window;
};

int main()
{
	std::cout << "Tutorial 02: Instance\n";
	HelloTriangleAplication app;

	try
	{
		app.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}