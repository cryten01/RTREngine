#pragma once
#include "Utils.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "SceneObject.h"
#include "JSONMapper.h"



/**
*	Manages the scene
*	Currently also manages the resources
**/

class Scene
{
private:
	std::shared_ptr<Shader> _standardShader;
	std::vector<std::shared_ptr<SceneObject>> _sceneObjects;

	void parseResource(const Value& resourceArray);
	void parseTextures(const Value& textureArray);
	void parseMaterials(const Value& materialArray);
	void parseMeshes(const Value& meshArray);

public:
	std::map<std::string, Texture> textureMap;
	std::map<std::string, std::shared_ptr<Material>> materialMap;
	std::map<std::string, std::shared_ptr<Mesh>> meshMap;

	 Scene(std::shared_ptr<Shader> standardShader);
	~Scene();

	void load(const std::string& filePath);
	void render();
	void update(float deltaTime);
};

