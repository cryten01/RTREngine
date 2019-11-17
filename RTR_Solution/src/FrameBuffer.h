#pragma once
#include "Utils.h"
#include "Shader.h"
#include "Mesh.h"

class FrameBuffer
{
private:
	GLuint _fbo;
	GLuint _rbo;
	GLuint _quadVAO;
	GLuint _quadVBO;
	GLuint _textureID;

	void loadMeshData();

public:
	 FrameBuffer(const GLuint WIDTH, const GLuint HEIGHT);
	~FrameBuffer();

	void use();
	void unuse();
	void renderScreenQuad(std::shared_ptr<Shader> shader);
};

