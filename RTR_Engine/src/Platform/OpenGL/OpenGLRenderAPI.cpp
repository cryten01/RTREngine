#include "RTRpch.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"

#include <GL/glew.h>


namespace RTREngine {

	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         RTR_CORE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       RTR_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          RTR_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: RTR_CORE_TRACE(message); return;
		}

		RTR_CORE_ASSERT(false, "Unknown severity level!");
	}


	void OpenGLRenderAPI::Init()
	{
#ifdef RTR_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		//glDebugMessageCallback(OpenGLMessageCallback, nullptr); // TODO
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}


	void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}


	void OpenGLRenderAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}


	void OpenGLRenderAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
