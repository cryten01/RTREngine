#include "RTRpch.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <GL/GL.h>

namespace RTREngine {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		RTR_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		RTR_CORE_INFO("OpenGL Info:");
		RTR_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		RTR_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		RTR_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}