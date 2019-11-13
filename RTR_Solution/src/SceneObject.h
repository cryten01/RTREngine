#pragma once
#include "Utils.h"
#include "Shader.h"
#include "SceneComponent.h"
#include "Transform.h"

class SceneObject
{
private:
	std::shared_ptr<Shader> _shader;
	std::list<SceneObject> _children;
	std::list<SceneComponent> _components;
	Transform _transform;

public:
	 SceneObject(std::shared_ptr<Shader> shader);
	~SceneObject();

	// Getter
	Transform& getTransform();

	// Setter
	void setShader(std::shared_ptr<Shader> shader);

	// Methods
	void addChild(SceneObject child);
	void addComponent(SceneComponent component);
};

