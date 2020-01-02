#pragma once
#include "Utils.h"
#include "Skybox.h"
#include "SceneObject.h"

class Scene
{
private:
	std::shared_ptr<Skybox> _skybox;
	std::vector<std::shared_ptr<SceneObject>> _sceneObjects;

	void setUniforms();

public:
	 Scene();
	~Scene();

	void load(const std::string& filePath);
	void update(float deltaTime);
	void render();
};