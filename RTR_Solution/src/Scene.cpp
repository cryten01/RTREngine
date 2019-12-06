#include "Scene.h"

Scene::Scene()
{
	// Read in json file
	std::string jsonString = Utils::readInFile("../assets/json/resources.json");
	const char* json = jsonString.c_str();

	// Parse json into document object as a DOM tree
	Document document;
	document.Parse(json);

	// Check if root is an object
	assert(document.IsObject());

	// Check if DOM tree has an array called textures and parse it
	assert(document.HasMember("textures"));
	assert(document["textures"].IsArray());
	parseTextures(document["textures"]);
}


Scene::~Scene()
{
}

void Scene::parseTextures(const Value& texArray)
{
	// Access texture objects attributes
	for (auto& texture : texArray.GetArray())
	{
		assert(texture.HasMember("name"));
		std::string name = texture["name"].GetString();

		assert(texture.HasMember("format"));
		std::string format = texture["format"].GetString();

		assert(texture.HasMember("type"));
		std::string type = texture["type"].GetString();

		// Create actual texture
		std::string total = "../assets/textures/" + name + format;
		Texture texture(total.c_str(), TEX_DIFFUSE);

		// Add texture to texture map
		textureMap.insert(std::pair<std::string, Texture>(name, texture));
	}
}


void Scene::render()
{
	for (std::shared_ptr<SceneObject> obj : sceneObjects)
	{
		obj->renderAll();
	}
}
