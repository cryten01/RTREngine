#include "Scene.h"

Scene::Scene(std::shared_ptr<Shader> standardShader)
	: _standardShader(standardShader)
{
	Document dom = JSONMapper::loadDom("../assets/json/resources.json");

	parseTextures(dom["textures"]);
	parseMaterials(dom["materials"]);
	parseMeshes(dom["meshes"]);
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
		assert(texture.HasMember("format"));
		assert(texture.HasMember("type"));

		std::string name = JSONMapper::getString(texture["name"]);
		std::string format = JSONMapper::getString(texture["format"]);
		TextureType type = JSONMapper::getTextureType(texture["type"]);

		// Create texture
		std::string total = "../assets/textures/" + name + format;
		Texture texture(total.c_str(), type);

		// Add texture to texture map
		textureMap.insert(std::pair<std::string, Texture>(name, texture));
	}
}

void Scene::parseMaterials(const Value & materialArray)
{
	// Access material objects attributes
	for (auto& jMaterial : materialArray.GetArray())
	{
		assert(jMaterial.HasMember("name"));
		assert(jMaterial.HasMember("shader"));
		assert(jMaterial.HasMember("type"));
		assert(jMaterial.HasMember("reflectionConstants"));
		assert(jMaterial.HasMember("alpha"));
		assert(jMaterial.HasMember("reflective"));
		assert(jMaterial.HasMember("refractive"));

		std::string name = JSONMapper::getString(jMaterial["name"]);
		MaterialType type = JSONMapper::getMaterialType(jMaterial["type"]);
		glm::vec3 reflectionConstants = JSONMapper::getVec3(jMaterial["reflectionConstants"]);
		float alpha = JSONMapper::getFloat(jMaterial["alpha"]);

		// Create actual material based on type
		std::shared_ptr<Material> material; 

		if (type == DIFFUSE) 
		{
			glm::vec3 diffuseColor = JSONMapper::getVec3(jMaterial["color"]);
			material = std::make_shared<Material>(_standardShader, reflectionConstants, alpha, diffuseColor);
		}
		else 
		{
			Texture& texture = JSONMapper::getTexture(textureMap, jMaterial["texture"]);
			material = std::make_shared<TextureMaterial>(_standardShader, reflectionConstants, alpha, texture);
		}

		// Set initial states
		material->setIsReflective(jMaterial["reflective"].GetBool());
		material->setIsRefractive(jMaterial["refractive"].GetBool());

		// Add material to material map
		materialMap.insert(std::pair<std::string, std::shared_ptr<Material>>(name, material));
	}
}

void Scene::parseMeshes(const Value & meshArray)
{
	for (auto& jMesh : meshArray.GetArray()) 
	{
		assert(jMesh.HasMember("name"));
		assert(jMesh.HasMember("type"));
		assert(jMesh.HasMember("dimensions"));
		assert(jMesh.HasMember("material"));

		std::string name = JSONMapper::getString(jMesh["name"]);
		std::shared_ptr<Material> material = JSONMapper::getMaterial(materialMap, jMesh["material"]);
		MeshData meshData = JSONMapper::getMeshData(jMesh["type"], jMesh["dimensions"]);

		// Create mesh
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(
			meshData,
			material
		);

		// Add mesh to mesh map
		meshMap.insert(std::pair<std::string, std::shared_ptr<Mesh>>(name, mesh));
	}
}


void Scene::render()
{
	for (std::shared_ptr<SceneObject> obj : _sceneObjects)
	{
		obj->renderAll();
	}
}
