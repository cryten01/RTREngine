#pragma once
#include "Utils.h"
#include "Interfaces.h"
#include "Shader.h"
#include "Transform.h"
#include "Mesh.h"
#include "Light.h"

/**
* 
**/
class SceneObject
{
private:
	bool _active;
	std::vector<std::shared_ptr<SceneObject>> _children;
	std::vector<std::shared_ptr<SceneComponent>> _components;
	std::shared_ptr<Transform> _transform;

public:
	 SceneObject(glm::mat4 modelMatrix);
	~SceneObject();

	// Getter & Setter
	std::shared_ptr<Transform> getTransform();
	void setTransform(std::shared_ptr<Transform> transform);

	// Methods
	void addChild(std::shared_ptr<SceneObject> child);
	void addComponent(std::shared_ptr<SceneComponent> component);

	void update(float deltaTime);
	void render(std::shared_ptr<Shader> shader);
};

