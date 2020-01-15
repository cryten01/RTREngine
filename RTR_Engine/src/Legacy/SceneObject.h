#pragma once
#include "Utils.h"
#include "Interfaces.h"
#include "Shader.h"
#include "Transform.h"

namespace RTREngine 
{
	class SceneObject
	{
	private:
		bool _active;
		std::vector<std::shared_ptr<SceneObject>> _children;
		std::vector<std::shared_ptr<SceneComponent>> _components;
		std::shared_ptr<Transform> _transform;

	public:
		SceneObject(glm::mat4 modelMatrix = glm::mat4(1.0f));
		~SceneObject();

		// Getter & Setter
		std::shared_ptr<Transform> getTransform();
		void setTransform(std::shared_ptr<Transform> transform);

		// Methods
		void addChild(std::shared_ptr<SceneObject> child);
		void addComponent(std::shared_ptr<SceneComponent> component);

		// Render & Update
		void update(float deltaTime);
		void setUniforms(std::shared_ptr<ShaderLegacy> shader);
	};
}