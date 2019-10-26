#pragma once

#include "Utils.h"
#include "Material.h"

struct GeometryData {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
};

class Geometry
{
protected:
	GLuint _vao;
	GLuint _vboPositions;
	GLuint _vboNormals;
	GLuint _vboUV;
	GLuint _vboIndices;
	unsigned int _elements;

	std::shared_ptr<Material> _material;

	glm::mat4 _modelMatrix;
	glm::mat4 _transformMatrix;

public:
	 Geometry(glm::mat4 modelMatrix, GeometryData& data, std::shared_ptr<Material> material);
	~Geometry();

	void draw();
	void transform(glm::mat4 transformation);
	void setTransformMatrix(glm::mat4 transformMatrix);
	void resetModelMatrix();

	static GeometryData createSphereGeometry(unsigned int longitudeSegments, unsigned int latitudeSegments, float radius);
	static float normalizeUV(float value, float min, float max);
};

