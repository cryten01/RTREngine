#pragma once
#include "Utils.h"
#include "SceneObject.h"
#include "Texture.h"

/**
*	Manages the scene
*	Currently also manages the resources
**/

class Scene
{
private:
	std::vector<std::shared_ptr<SceneObject>> sceneObjects;

	void parseTextures(const Value& texArray);

public:
	std::map<std::string, Texture> textureMap;

	 Scene();
	~Scene();

	void load(const std::string& filePath);
	void render();
	void update(float deltaTime);
};

