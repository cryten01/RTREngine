#include "SceneLayer.h"
#include "Keymappings.h"


namespace WinterWonderland {

	SceneLayer::SceneLayer()
		: RTREngine::Layer("WinterWonderland")
	{
	};


	void SceneLayer::OnUpdate(RTREngine::Time deltaTime)
	{
		WinterWonderland::TriggerKeyCallback();

		// Render
		{
			RTREngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RTREngine::RenderCommand::Clear();
		}
	}


	void SceneLayer::createTriangle(RTREngine::Ref<RTREngine::VertexArray> vertexArray)
	{
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		uint32_t indices[3] = { 0, 1, 2 };

		vertexArray = RTREngine::VertexArray::Create();

		RTREngine::Ref<RTREngine::VertexBuffer> vertexBuffer = RTREngine::VertexBuffer::Create(vertices, sizeof(vertices));

		RTREngine::BufferLayout layout = {
			{ RTREngine::BufferDataType::Vec3, "a_Position" },
			{ RTREngine::BufferDataType::Vec4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		vertexArray->AddVertexBuffer(vertexBuffer);

		RTREngine::Ref<RTREngine::IndexBuffer> indexBuffer = RTREngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		vertexArray->SetIndexBuffer(indexBuffer);
	}
}