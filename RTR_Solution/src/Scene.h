#pragma once
#include "Utils.h"
#include "SceneObject.h"
#include "Texture.h"
#include "Shader.h"

/**
*	Manages the scene
*	Currently also manages the resources
**/

class Scene
{
private:
	std::shared_ptr<Shader> _standardShader;
	std::vector<std::shared_ptr<SceneObject>> _sceneObjects;

	void parseTextures(const Value& textureArray);
	void parseMaterials(const Value& materialArray);

public:
	std::map<std::string, Texture> textureMap;
	std::map<std::string, std::shared_ptr<Material>> materialMap;

	 Scene(std::shared_ptr<Shader> standardShader);
	~Scene();

	void load(const std::string& filePath);
	void render();
	void update(float deltaTime);
};

