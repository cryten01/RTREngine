#include "Window.h"

using namespace RTREngine;

Window::Window(const char* title, const int width, const int height)
	: _TITLE(title), _WIDTH(width), _HEIGHT(height)
{
	this->init();
}


Window::~Window()
{
}


void RTREngine::Window::setActiveScene(std::shared_ptr<Scene> scene)
{
	this->_activeScene = scene;
}


/**
*	Updates the window and its currently active scene
**/
void Window::update(float deltaTime)
{
	//_activeScene->update(deltaTime);
}


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

	// Set per-frame uniforms
	//setPerFrameUniforms(standardShader, orbitCam, dirLight, pointLights, spotLights, skybox);

	// Switch to screnQuadBuffer
	//hdrBuffer.use();

	// Render scene
	// Scene::render();

	// Switch back to default buffer
	//hdrBuffer.unuse();

	/**************************************************/
	//	Second render pass (render buffer to quad)
	/**************************************************/

	//hdrBuffer.renderScreenQuad(postProcessShader, _hdr, _exposure);
}

GLFWwindow* Window::getGLFWWindow()
{
	return this->_window;
}

const int Window::getWidth()
{
	return this->_WIDTH;
}

const int Window::getHeight()
{
	return this->_HEIGHT;
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
	_window = glfwCreateWindow(_WIDTH, _HEIGHT, _TITLE, NULL, NULL);
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