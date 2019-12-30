#include "Window.h"

Window::Window(const char* title, const int width, const int height)
	: TITLE(title), WIDTH(width), HEIGHT(height)
{
	this->create();
}


Window::~Window()
{
}

GLFWwindow* Window::getGLFWWindow()
{
	return this->_window;
}

const int Window::getWidth()
{
	return this->WIDTH;
}

const int Window::getHeight()
{
	return this->HEIGHT;
}


int Window::create()
{
	// Initialize GLFW library
	if (!glfwInit()) {
		EXIT_WITH_ERROR("Failed to init GLFW")
	}

	// Set required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // window size fixed
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Create a GLFWwindow object is used for GLFW's functions
	_window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
	if (!_window)
	{
		glfwTerminate();
		EXIT_WITH_ERROR("Failed to create window")
	}

	// Make the window's context current
	glfwMakeContextCurrent(_window);


	// Everything is ok (for debugging only)
	return EXIT_SUCCESS;
}