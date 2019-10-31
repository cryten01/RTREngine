#include "Debugmesh.h"


/**
*	Constructor, loading the specified aiMesh
**/
Debugmesh::MeshEntry::MeshEntry(aiMesh *mesh) {
	
	GeometryData data = loadGeometry(mesh);
	elementCount = data.indices.size();


	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);


	glGenBuffers(1, &_vboPositions);
	glBindBuffer(GL_ARRAY_BUFFER, _vboPositions);
	glBufferData(GL_ARRAY_BUFFER, data.positions.size() * sizeof(glm::vec3), data.positions.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);


	glGenBuffers(1, &_vboUV);
	glBindBuffer(GL_ARRAY_BUFFER, _vboUV);
	glBufferData(GL_ARRAY_BUFFER, data.uv.size() * sizeof(glm::vec2), data.uv.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);


	glGenBuffers(1, &_vboNormals);
	glBindBuffer(GL_ARRAY_BUFFER, _vboNormals);
	glBufferData(GL_ARRAY_BUFFER, data.normals.size() * sizeof(glm::vec3), data.normals.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);


	glGenBuffers(1, &_vboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(GLuint), data.indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(3);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GeometryData Debugmesh::MeshEntry::loadGeometry(aiMesh *mesh)
{
	GeometryData data;

	if (mesh->HasPositions() && mesh->HasNormals() && mesh->HasTextureCoords(0)) {
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			glm::vec3 vertexPos;
			vertexPos.x = mesh->mVertices[i].x;
			vertexPos.y = mesh->mVertices[i].y;
			vertexPos.z = mesh->mVertices[i].z;
			data.positions.push_back(vertexPos);

			glm::vec2 uv;
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;
			data.uv.push_back(uv);

			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			data.normals.push_back(normal);
		}
	}

	if (mesh->HasFaces()) {
		for (int i = 0; i < mesh->mNumFaces; ++i) {
			data.indices.push_back(mesh->mFaces[i].mIndices[0]);
			data.indices.push_back(mesh->mFaces[i].mIndices[1]);
			data.indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
	}

	return std::move(data);
};

void Debugmesh::MeshEntry::createMesh(GeometryData& data)
{

};


/**
*	Deletes the allocated OpenGL buffers
**/
Debugmesh::MeshEntry::~MeshEntry() {
	glDeleteBuffers(1, &_vboPositions);
	glDeleteBuffers(1, &_vboNormals);
	glDeleteBuffers(1, &_vboUV);
	glDeleteBuffers(1, &_vboIndices);
	glDeleteVertexArrays(1, &_vao);
}

/**
*	Renders this MeshEntry
**/
void Debugmesh::MeshEntry::render(std::shared_ptr<Shader> shader) {

	// Select shader
	shader->use();

	// Set uniforms
	shader->setUniform("modelMatrix", glm::mat4(1.0f));
	shader->setUniform("diffuseColor", glm::vec3(1,0,1));

	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}






/**
*	Mesh constructor, loads the specified filename if supported by Assimp
**/
Debugmesh::Debugmesh(const char *filename)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);;
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	for (int i = 0; i < scene->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[i];
		meshEntries.push_back(new Debugmesh::MeshEntry(mesh));
	}
}

/**
*	Clears all loaded MeshEntries
**/
Debugmesh::~Debugmesh(void)
{
	for (int i = 0; i < meshEntries.size(); ++i) {
		delete meshEntries.at(i);
	}
	meshEntries.clear();
}

/**
*	Renders all loaded MeshEntries
**/
void Debugmesh::render(std::shared_ptr<Shader> shader) {
	for (int i = 0; i < meshEntries.size(); ++i) {
		meshEntries.at(i)->render(shader);
	}
}