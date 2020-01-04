#include "Window.h"

using namespace RTREngine;

Window::Window(const char* title, const int width, const int height)
	: _title(title), _width(width), _height(height)
{
	this->init();
}


Window::~Window()
{
}


int Window::init()
{
	// Set required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // window size fixed
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Create a GLFWwindow object is used for GLFW's functions
	_window = glfwCreateWindow(_width, _height, _title, NULL, NULL);
	if (!_window)
	{
		glfwTerminate();
		EXIT_WITH_ERROR("Failed to create window")
	}

	// Make the window's context current
	glfwMakeContextCurrent(_window);

	// Everything is ok
	return EXIT_SUCCESS;
}


GLFWwindow* Window::getGLFWWindow()
{
	return this->_window;
}


const int Window::getWidth()
{
	return this->_width;
}


const int Window::getHeight()
{
	return this->_height;
}


void Window::setActiveScene(std::shared_ptr<Scene> scene)
{
	this->_activeScene = scene;
}


/**
*	Updates all content thats inside the window
**/
void Window::update(float deltaTime)
{
	_activeScene->update(deltaTime);
}

/**
*	Renders all content thats inside the window
**/
void Window::render()
{
	/********************/
	//	Preparations
	/********************/

	// Enable if default buffer is used only!
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/**************************************************************/
	//	First render pass (render scene into screenQuadBuffer)	
	/**************************************************************/

	Resources& resources = Resources::Instance();

	// Switch to screnQuadBuffer
	//resources.hdrBuffer->use();

	// Render scene
	_activeScene->render(resources.standardShader);

	// Switch back to default buffer
	//resources.hdrBuffer->unuse();

	/**************************************************/
	//	Second render pass (render buffer to quad)
	/**************************************************/

	// Variables for debugging only!
	bool hdr = true;
	bool exposure = false;

	//Resources::Instance().hdrBuffer->renderScreenQuad(resources.postProcessShader, hdr, exposure);
}