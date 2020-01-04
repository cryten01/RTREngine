#pragma once

#include "Utils.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"


namespace RTREngine 
{
	static const std::string& SKYBOX_ROOT_LOCATION = "../assets/textures/";

	class Skybox
	{
	protected:
		std::shared_ptr<Camera> _camera;

		GLuint _vao;
		GLuint _vboPositions;
		GLuint _vboIndices;
		GLuint _cubeMapID;
		unsigned int _elementCount;

		MeshData createBoxGeometry(float size);
		void loadTextures(std::vector<std::string>& textureFileNames);
		void bindTextures(unsigned int unit = 7);

	public:
		 Skybox(std::shared_ptr<Camera> camera, float size, std::vector<std::string>& textureFileNames);
		~Skybox();

		void render(std::shared_ptr<Shader> shader);
	};
}