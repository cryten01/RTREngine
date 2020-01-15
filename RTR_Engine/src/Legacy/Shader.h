#pragma once
#include "Utils.h"

namespace RTREngine
{
	static const std::string& SHADER_ROOT_LOCATION = "../assets/shader/";

	class ShaderLegacy
	{
	private:
		GLuint _programID;

		GLuint compileShader(const std::string& filePath, GLuint type);
		void addToProgram(GLenum shaderType, const std::string& filePath);

	public:
		ShaderLegacy(const std::string& fileName, const std::string& fragmentFilePath, const std::string& geometryFilePath = "");
		ShaderLegacy(const std::string& computeFilePath);
		~ShaderLegacy();

		void use() const;
		void unuse() const;
		GLint getUniformLocation(const std::string& name);

		void setUniform(GLint location, const glm::vec3& vec);
		void setUniform(GLint location, const glm::vec4& vec);

		void setUniform(std::string uniform, const int i);
		void setUniform(std::string uniform, const unsigned int i);
		void setUniform(std::string uniform, const float f);
		void setUniform(std::string uniform, const glm::vec3& vec);
		void setUniform(std::string uniform, const glm::mat3& mat);
		void setUniform(std::string uniform, const glm::mat4& mat);
	};
}