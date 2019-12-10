#include "Scene.h"

Scene::Scene(std::shared_ptr<Shader> standardShader)
	: _standardShader(standardShader)
{
	Document dom = JMapper::loadDom("../assets/json/resources.json");

	JMapper::checkObjectStructure(dom["textures"], { "name", "format", "type" });
	parseTextures(dom["textures"]);
	parseMaterials(dom["materials"]);
	parseMeshes(dom["meshes"]);
}


Scene::~Scene()
{
}

void Scene::parseTextures(const Value& objectArray)
{
	// Retrieve objects
	for (auto& object : objectArray.GetArray())
	{
		JMapper mapper(object);
		auto name = mapper.getString("name");
		auto format = mapper.getString("format");
		auto type = mapper.getTextureType("type");

		// Create texture
		std::string filePath = "../assets/textures/" + name + format;
		Texture texture(filePath.c_str(), type);

		// Add texture to texture map
		textureMap.insert(std::pair<std::string, Texture>(name, texture));
	}
}


void Scene::parseMaterials(const Value & materialArray)
{
	// Access material objects attributes
	for (auto& jMaterial : materialArray.GetArray())
	{
		JMapper mapper(jMaterial);
		auto name = mapper.getString("name");
		auto reflectionConstants = mapper.getVec3("reflectionConstants");
		auto alpha = mapper.getFloat("alpha");
		auto refractive = mapper.getBool("refractive");
		auto reflective = mapper.getBool("reflective");

		// Create material
		std::shared_ptr<Material> material;

		if (jMaterial.HasMember("color")) 
		{
			auto diffuseColor = mapper.getVec3("color");
			material = std::make_shared<Material>(_standardShader, reflectionConstants, alpha, diffuseColor);
		}
		else 
		{
			auto texture = mapper.lookupTexture("texture", textureMap);
			material = std::make_shared<TextureMaterial>(_standardShader, reflectionConstants, alpha, texture);
		}
		
		// Set initial states
		material->setIsReflective(reflective);
		material->setIsRefractive(refractive);

		// Add material to material map
		materialMap.insert(std::pair<std::string, std::shared_ptr<Material>>(name, material));
	}
}

void Scene::parseMeshes(const Value & meshArray)
{
	for (auto& jMesh : meshArray.GetArray()) 
	{
		JMapper mapper(jMesh);
		auto name = mapper.getString("name");		
		auto material = mapper.lookupMaterial("material", materialMap);
		auto data = mapper.getMeshData("type", "dimensions");

		// Create mesh
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(data, material);

		// Add mesh to mesh map
		meshMap.insert(std::pair<std::string, std::shared_ptr<Mesh>>(name, mesh));
	}
}


//
//void Scene::JMaterial(const Value& object)
//{
//	std::list<const char*> constructorParameter = { "name", "shader", "reflectionConstants", "alpha", "reflective", "refractive" };
//
//	for (const char* parameter : constructorParameter)
//	{
//		assert(object.HasMember(parameter));
//	}
//
//	for (Value::ConstMemberIterator itr = object.MemberBegin(); itr != object.MemberEnd(); ++itr)
//	{
//		printf("Type of member %s", itr->name.GetString());
//	}
//}

void Scene::render()
{
	for (std::shared_ptr<SceneObject> obj : _sceneObjects)
	{
		obj->renderAll();
	}
}
