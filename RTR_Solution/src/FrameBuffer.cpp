#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(const GLuint WIDTH, const GLuint HEIGHT, BufferType type)
	: _type(type)
{
	// Create framebuffer object
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	// Create color attachment texture
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	
	// Give an empty image to OpenGL (therefore NULL at the end)
	if (type == DEFAULT) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);		
	}
	if (type == FLOAT) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);								// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);								// Poor filtering. Needed !
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _textureID, 0);						// Attach texture to framebuffer (as colour attachement #0) 


	// Create renderBuffer object for depth
	glGenRenderbuffers(1, &_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);				// Creates actual RBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rbo);	// Attach RBO to framebuffer
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Check if the framebuffer was successfully completed or not
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Failed to complete framebuffer" << std::endl;
	}

	// Unbind (so no accidental rendering to the wrong framebuffer is possible)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	

	// Loads screen quad
	loadScreenQuad();
}


FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &_fbo);
	glDeleteRenderbuffers(1, &_rbo);
	glDeleteTextures(1, &_textureID);
	glDeleteVertexArrays(1, &_quadVAO);
	glDeleteBuffers(1, &_quadVBO);
}


void FrameBuffer::loadScreenQuad()
{
	// vertex attributes for the screenQuad (fills entire screen in normalized device coordinates)
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// create VAO
	glGenVertexArrays(1, &_quadVAO);
	glBindVertexArray(_quadVAO);

	// create VBO
	glGenBuffers(1, &_quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	// bind positions to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	// bind uvs to location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void FrameBuffer::use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::unuse()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::renderScreenQuad(std::shared_ptr<Shader> shader)
{
	shader->use();

	// Set additional uniforms if type is HDR
	if (_type == FLOAT) 
	{
		shader->setUniform("hdr", false);
		shader->setUniform("exposure", 0.1f);
	}

	glDisable(GL_DEPTH_TEST); // disable depth test so screen quad isn't discarded
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glBindVertexArray(_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
