#pragma once

#include "Utils.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"

class Debugmesh
{
public:
	struct MeshEntry {
		GLuint _vao;
		GLuint _vboPositions;
		GLuint _vboNormals;
		GLuint _vboUV;
		GLuint _vboIndices;

		unsigned int elementCount;

		 MeshEntry(aiMesh *mesh);
		~MeshEntry();

		GeometryData loadGeometry(aiMesh *mesh);
		void createMesh(GeometryData& data);

		void render(std::shared_ptr<Shader> shader);
	};

	std::vector<MeshEntry*> meshEntries;

public:
	 Debugmesh(const char *filename);
	~Debugmesh(void);

	void render(std::shared_ptr<Shader> shader);
};
