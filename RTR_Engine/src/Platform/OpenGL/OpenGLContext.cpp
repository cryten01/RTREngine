#include "RTRpch.h"

#include "Platform/OpenGL/OpenGLContext.h"

#define GLEW_STATIC
#include <GL/glew.h> // must be added before GL and glfw!
#include <GL/GL.h>		
#include <GLFW/glfw3.h> 

namespace RTREngine {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		RTR_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		// If true GLEW uses a modern approach for retrieving function pointers and extensions
		glewExperimental = true;

		// Initialize GLEW to setup OpenGL function pointers
		int status = glewInit();
		RTR_CORE_ASSERT(status == GLEW_OK, "Failed to init GLEW!")
	
		RTR_CORE_INFO("OpenGL Info:");
		RTR_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		RTR_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		RTR_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));


#ifdef RTR_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		RTR_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 3), "RTREngine requires at least OpenGL version 4.3!");
#endif

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}