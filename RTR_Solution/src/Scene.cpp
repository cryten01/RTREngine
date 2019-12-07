#include "Scene.h"

Scene::Scene(std::shared_ptr<Shader> standardShader)
	: _standardShader(standardShader)
{
	// Read in json file
	std::string jsonString = Utils::readInFile("../assets/json/resources.json");
	const char* json = jsonString.c_str();

	// Parse json into document object as a DOM tree
	Document document;
	document.Parse(json);

	// Check if root is an object
	assert(document.IsObject());

	// Check if DOM tree contains a texture array and parse it
	assert(document.HasMember("textures"));
	assert(document["textures"].IsArray());
	parseTextures(document["textures"]);

	// Check if DOM tree contains a material array and parse it
	assert(document.HasMember("materials"));
	assert(document["materials"].IsArray());
	parseMaterials(document["materials"]);
}


Scene::~Scene()
{
}

void Scene::parseTextures(const Value& textureArray)
{
	// Access texture objects attributes
	for (auto& texture : textureArray.GetArray())
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

void Scene::parseMaterials(const Value & materialArray)
{
	// Access material objects attributes
	for (auto& material : materialArray.GetArray())
	{
		assert(material.HasMember("name"));
		std::string name = material["name"].GetString();

		assert(material.HasMember("shader"));
		std::string format = material["shader"].GetString();

		assert(material.HasMember("type"));
		std::string type = material["type"].GetString();

		assert(material.HasMember("reflectionConstants"));
		const Value& constants = material["reflectionConstants"];
		glm::vec3 reflectionConstants = glm::vec3(constants[0].GetFloat(), constants[1].GetFloat(), constants[2].GetFloat());

		assert(material.HasMember("alpha"));
		float alpha = material["alpha"].GetFloat();


		// Create actual material based on type
		std::shared_ptr<Material> mat; 

		if (type == "color") 
		{
			const Value& color = material["color"];
			glm::vec3 diffuseColor = glm::vec3(color[0].GetFloat(), color[1].GetFloat(), color[2].GetFloat());
			mat = std::make_shared<Material>(_standardShader, reflectionConstants, alpha, diffuseColor);
		}
		else 
		{
			const Value& texture = material["texture"];
			Texture& tex = textureMap.find(texture.GetString())->second;
			mat = std::make_shared<TextureMaterial>(_standardShader, reflectionConstants, alpha, tex);
		}

		// Set initial states
		assert(material.HasMember("reflective"));
		mat->setIsReflective(material["reflective"].GetBool());

		assert(material.HasMember("refractive"));
		mat->setIsRefractive(material["refractive"].GetBool());

		// Add material to material map
		materialMap.insert(std::pair<std::string, std::shared_ptr<Material>>(name, mat));
	}
}


void Scene::render()
{
	for (std::shared_ptr<SceneObject> obj : _sceneObjects)
	{
		obj->renderAll();
	}
}
