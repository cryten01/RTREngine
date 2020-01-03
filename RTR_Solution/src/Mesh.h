#pragma once
#include "Utils.h"
#include "Material.h"
#include "Interfaces.h"

namespace RTREngine
{
	/** Represents a mesh containing vertex positions, normal vectors, texture coordinates and material*/

	struct MeshData {
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uv;
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

	public:
		Mesh(MeshData data, std::shared_ptr<Material> material);
		~Mesh();

		void render();
		std::shared_ptr<Material> getMaterial();

		static float normalizeUV(float value, float min, float max);
		static MeshData createCubeGeometry(float width, float height, float depth);
		static MeshData createCylinderGeometry(unsigned int segments, float height, float radius);
		static MeshData createSphereGeometry(unsigned int longitudeSegments, unsigned int latitudeSegments, float radius);
	};
}

