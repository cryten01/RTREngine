#pragma once
#include "Utils.h"
#include "SceneObject.h"

class Scene
{
private:
	std::vector<std::shared_ptr<SceneObject>> _sceneObjects;

public:
	 Scene();
	~Scene();

	void load(const std::string& filePath);
	void render();
	void update(float deltaTime);
};