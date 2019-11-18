#pragma once
#include "Utils.h"
#include "Shader.h"
#include "Mesh.h"

enum BufferType
{
	DEFAULT,
	FLOAT,		// can store floating point values outside the default range of 0.0 and 1.0.
};


class FrameBuffer
{
private:
	GLuint _fbo;
	GLuint _rbo;
	GLuint _quadVAO;
	GLuint _quadVBO;
	GLuint _textureID;
	BufferType _type;

	void loadScreenQuad();

public:
	 FrameBuffer(const GLuint WIDTH, const GLuint HEIGHT, BufferType type);
	~FrameBuffer();

	void use();
	void unuse();

	void renderScreenQuad(std::shared_ptr<Shader> shader, bool hdr, float exposure);
};

