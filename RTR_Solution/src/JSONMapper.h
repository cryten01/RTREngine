#pragma once
#include "Utils.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

/**
*	A mapper for converting JSON attribute values to (interpreted) C++ values 
*	This mapper extends the rapidJSON library
**/
class JSONMapper
{
public:
	 JSONMapper();
	~JSONMapper();

	// Helper methods
	static Document loadDom(const std::string& jFilePath);

	// Basic types (from RapidJSON)
	static float getFloat(const Value& jFloat);
	static std::string getString(const Value& jString);

	// Extended types
	static MeshData getMeshData(const Value& jType, const Value& jVecArray);
	static TextureType getTextureType(const Value& jType);
	static MaterialType getMaterialType(const Value& jType);
	static Texture& getTexture(std::map<std::string, Texture>& map, const Value& jKey);
	static std::shared_ptr<Material> getMaterial(std::map<std::string, std::shared_ptr<Material>>& map, const Value& jKey);
	static glm::vec3 getVec3(const Value& jVecArray);
};

