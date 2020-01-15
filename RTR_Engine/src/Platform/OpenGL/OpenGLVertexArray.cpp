#include "rtrpch.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <GL/glew.h>

namespace RTREngine {

	static GLenum BufferDataTypeToOpenGLBaseType(BufferDataType type)
	{
		switch (type)
		{
		case BufferDataType::Float:		return GL_FLOAT;
		case BufferDataType::Vec2:		return GL_FLOAT;
		case BufferDataType::Vec3:		return GL_FLOAT;
		case BufferDataType::Vec4:		return GL_FLOAT;
		case BufferDataType::Mat3:		return GL_FLOAT;
		case BufferDataType::Mat4:		return GL_FLOAT;
		case BufferDataType::Int:		return GL_INT;
		case BufferDataType::Bool:		return GL_BOOL;
		}

		RTR_CORE_ASSERT(false, "Unknown BufferDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		RTR_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex,
				element.GetComponentCount(),
				BufferDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			m_VertexBufferIndex++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}
