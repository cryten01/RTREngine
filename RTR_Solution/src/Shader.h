#pragma once

#include "Utils.h"

class Shader
{
private:
	GLuint _programID;
	//std::vector<GLuint> _shaderIDs;

	std::string readInFile(const std::string& filePath);
	GLuint compileShader(const std::string& filePath, GLuint type);

public:
	 Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
	~Shader();

	void use() const;

	GLint getUniformLocation(const std::string& name);
	void setUniform(GLint location, const glm::vec3& vec);
	void setUniform(GLint location, const glm::vec4& vec);
	void setUniform(std::string uniform, const float f);
	void setUniform(std::string uniform, const unsigned int i);
	void setUniform(std::string uniform, const glm::vec3& vec);
	void setUniform(std::string uniform, const glm::mat4& mat);
};

