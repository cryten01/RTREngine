#include "Framework.h"

Framework::Framework()
{
}

Framework::~Framework()
{
}


void Framework::startRenderLoop()
{
	currentTime = float(glfwGetTime());
	lastTime = float(glfwGetTime());
	deltaTime = 0.0f;
	runTime = 0.0f;
	frames = 0;
	fps = 0;

	while (!glfwWindowShouldClose(window->getGLFWWindow()))
	{
		update();
		render();

		// Poll events and swap buffers
		Controls::key_polling(window->getGLFWWindow(), deltaTime);
		glfwPollEvents();
		glfwSwapBuffers(window->getGLFWWindow());
	}
}

void Framework::update()
{
	// Compute frame time
	deltaTime = currentTime;
	currentTime = float(glfwGetTime());
	deltaTime = currentTime - deltaTime;
	runTime += deltaTime;

	// FPS counter
	frames++;
	if (currentTime - lastTime >= 1.0) {
		fps = frames;
		frames = 0;
		lastTime += 1.0;
	}

	// Update current scene here (for debugging only!)
	//testScene.updateScene(deltaTime, controls);
}

void Framework::render()
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

	// Init window
	window = std::make_unique<Window>("RTR Engine", 800, 600);

	// Init controls (must be after GLFW window creation)
	Controls::init(window->getGLFWWindow());

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