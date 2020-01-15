#pragma once
#include "Utils.h"
#include "Shader.h"
#include "Mesh.h"

namespace RTREngine
{
	enum BufferType
	{
		DEFAULT,
		FLOAT,		// can store floating point values outside the default range of 0.0 and 1.0.
	};


	class FrameBuffer
	{
	private:
		const GLuint _WIDTH, _HEIGHT;

		GLuint _fbo;
		GLuint _rbo;
		GLuint _quadVAO;
		GLuint _quadVBO;
		GLuint _textureID;
		BufferType _type;

		void addTexture(GLenum texFormat, GLenum dataFormat);
		void addRBO();
		void loadScreenQuad();

	public:
		FrameBuffer(const GLuint WIDTH, const GLuint HEIGHT, BufferType type);
		~FrameBuffer();

		void use();
		void unuse();

		void renderScreenQuad(std::shared_ptr<ShaderLegacy> shader, bool hdr, float exposure);
	};
}
