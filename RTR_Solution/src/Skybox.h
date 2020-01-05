#pragma once

#include "Utils.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Interfaces.h"

namespace RTREngine 
{
	static const std::string& SKYBOX_ROOT_LOCATION = "../assets/textures/";
	static const int CUBEMAP_UNIT = 5;

	class Skybox : public Renderable, Uniformable
	{
	protected:
		std::shared_ptr<Camera> _camera;

		GLuint _vao;
		GLuint _vboPositions;
		GLuint _vboIndices;
		unsigned int _elementCount;
		GLuint _cubeMapID;

		MeshData createBoxGeometry(float size);
		void loadTextures(std::vector<std::string>& textureFileNames);
		void bindTextures(unsigned int unit);

	public:
		 Skybox(std::shared_ptr<Camera> camera, float size, std::vector<std::string>& textureFileNames);
		~Skybox();

		void setUniforms(std::shared_ptr<Shader> shader) override;
		void render(std::shared_ptr<Shader> shader) override;
	};
}