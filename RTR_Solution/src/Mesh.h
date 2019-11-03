#pragma once

#include "Utils.h"
#include "Material.h"

/** Represents a mesh containing vertex positions, normal vectors, texture coordinates and material*/

struct GeometryData {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> indices;
};

class Mesh
{
protected:
	GLuint _vao;
	GLuint _vboPositions;
	GLuint _vboNormals;
	GLuint _vboUV;
	GLuint _vboIndices;
	unsigned int _elementCount;

	std::shared_ptr<Material> _material;

	glm::mat4 _modelMatrix;
	glm::mat4 _transformMatrix;

public:
	 Mesh(glm::mat4 modelMatrix, GeometryData& data, std::shared_ptr<Material> material);
	~Mesh();

	void render();
	void transform(glm::mat4 transformation);
	void resetModelMatrix();

	std::shared_ptr<Material> getMaterial();
	void setTransformMatrix(glm::mat4 transformMatrix);

	static float normalizeUV(float value, float min, float max);
	static GeometryData createCubeGeometry(float width, float height, float depth);
	static GeometryData createSphereGeometry(unsigned int longitudeSegments, unsigned int latitudeSegments, float radius);

};

