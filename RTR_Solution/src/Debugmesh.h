#pragma once

#include "Utils.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"

class Debugmesh
{
public:
	struct MeshEntry {
		static enum BUFFERS {
			VERTEX_BUFFER, 
			TEXCOORD_BUFFER, 
			NORMAL_BUFFER, 
			INDEX_BUFFER
		};
		GLuint vao;
		GLuint vbo[4];

		unsigned int elementCount;

		 MeshEntry(aiMesh *mesh);
		~MeshEntry();

		void load(aiMesh *mesh);
		void render(std::shared_ptr<Shader> shader);
	};

	std::vector<MeshEntry*> meshEntries;

public:
	 Debugmesh(const char *filename);
	~Debugmesh(void);

	void render(std::shared_ptr<Shader> shader);
};
