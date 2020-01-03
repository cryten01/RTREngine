#include "Framework.h"

using namespace RTREngine;


Framework::Framework()
{
}


Framework::~Framework()
{
}


/**
*	Inits OpenGL framework
*	Must return int because of EXIT_WITH ERROR
**/
int Framework::init()
{
	// Initialize GLFW
	if (!glfwInit()) {
		EXIT_WITH_ERROR("Failed to init GLFW")
	}

	window = std::make_shared<Window>("RTR Engine", 800, 600);
	input = std::make_unique<Input>(window->getGLFWWindow());
	time = std::make_unique<Time>();

	// GLEW init
	glewExperimental = true; // if true GLEW uses a modern approach for retrieving function pointers and extensions

	// Initialize GLEW to setup OpenGL function pointers
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		EXIT_WITH_ERROR("Failed to init GLEW")
	}

	// Set GL defaults here
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if _DEBUG
	// Register your callback function.
	glDebugMessageCallback(DebugCallbackDefault, NULL);
	// Enable synchronous callback. This ensures that your callback function is called
	// right after an error has occurred. 
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
}


/**
*	Terminates OpenGL framework
*	Must return int because of EXIT_SUCCESS
**/
int Framework::destroy()
{
	glfwTerminate();
	return EXIT_SUCCESS;
}

std::shared_ptr<Window> Framework::getWindow()
{
	return this->window;
}


/**
*	Start render loop
**/
void Framework::startRenderLoop()
{
	// Init render loop
	while (!glfwWindowShouldClose(window->getGLFWWindow()))
	{
		time->update();
		window->update(time->getDeltaTime());
		window->render();

		// Poll events and swap buffers
		input->key_polling(window->getGLFWWindow(), time->getDeltaTime());
		glfwPollEvents();
		glfwSwapBuffers(window->getGLFWWindow());
	}
}


void APIENTRY Framework::DebugCallbackDefault(GLenum source, GLenum textype, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) {
	if (id == 131185 || id == 131218) return; // ignore performance warnings (buffer uses GPU memory, shader recompilation) from nvidia
	std::string error = FormatDebugOutput(source, textype, id, severity, message);
	std::cout << error << std::endl;
}


std::string Framework::FormatDebugOutput(GLenum source, GLenum textype, GLuint id, GLenum severity, const char* msg) {
	std::stringstream stringStream;
	std::string sourceString;
	std::string typeString;
	std::string severityString;

	switch (source) {
	case GL_DEBUG_SOURCE_API: {
		sourceString = "API";
		break;
	}
	case GL_DEBUG_SOURCE_APPLICATION: {
		sourceString = "Application";
		break;
	}
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
		sourceString = "Window System";
		break;
	}
	case GL_DEBUG_SOURCE_SHADER_COMPILER: {
		sourceString = "Shader Compiler";
		break;
	}
	case GL_DEBUG_SOURCE_THIRD_PARTY: {
		sourceString = "Third Party";
		break;
	}
	case GL_DEBUG_SOURCE_OTHER: {
		sourceString = "Other";
		break;
	}
	default: {
		sourceString = "Unknown";
		break;
	}
	}

	switch (textype) {
	case GL_DEBUG_TYPE_ERROR: {
		typeString = "Error";
		break;
	}
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
		typeString = "Deprecated Behavior";
		break;
	}
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
		typeString = "Undefined Behavior";
		break;
	}
	case GL_DEBUG_TYPE_PORTABILITY_ARB: {
		typeString = "Portability";
		break;
	}
	case GL_DEBUG_TYPE_PERFORMANCE: {
		typeString = "Performance";
		break;
	}
	case GL_DEBUG_TYPE_OTHER: {
		typeString = "Other";
		break;
	}
	default: {
		typeString = "Unknown";
		break;
	}
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: {
		severityString = "High";
		break;
	}
	case GL_DEBUG_SEVERITY_MEDIUM: {
		severityString = "Medium";
		break;
	}
	case GL_DEBUG_SEVERITY_LOW: {
		severityString = "Low";
		break;
	}
	default: {
		severityString = "Unknown";
		break;
	}
	}

	stringStream << "OpenGL Error: " << msg;
	stringStream << " [Source = " << sourceString;
	stringStream << ", Type = " << typeString;
	stringStream << ", Severity = " << severityString;
	stringStream << ", ID = " << id << "]";

	return stringStream.str();
}