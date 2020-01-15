#include "rtrpch.h"
#include "Shader.h"

using namespace RTREngine;

/**
*	Creates a shader porgram. Needs at least one shader
**/
ShaderLegacy::ShaderLegacy(const std::string& vertexFileName, const std::string& fragmentFileName, const std::string& geometryFileName)
{
	_programID = glCreateProgram();
	addToProgram(GL_VERTEX_SHADER, vertexFileName);
	addToProgram(GL_FRAGMENT_SHADER, fragmentFileName);

	if (geometryFileName != "")
		addToProgram(GL_GEOMETRY_SHADER, geometryFileName);

	// Linking and validation
	glLinkProgram(_programID);
	glValidateProgram(_programID);
}


ShaderLegacy::ShaderLegacy(const std::string & computeFileName)
{
	_programID = glCreateProgram();
	addToProgram(GL_COMPUTE_SHADER, computeFileName);

	// Linking and validation
	glLinkProgram(_programID);
	glValidateProgram(_programID);
}


ShaderLegacy::~ShaderLegacy()
{
	glUseProgram(0);
	glDeleteProgram(_programID);
}


void ShaderLegacy::addToProgram(GLenum shaderType, const std::string& fileName)
{
	GLuint shaderID;

	// Compile shader
	std::string shaderSource = Utils::readInFile(SHADER_ROOT_LOCATION + fileName);
	shaderID = compileShader(shaderSource, shaderType);

	// Attach shader
	glAttachShader(_programID, shaderID);

	// Delete shader
	glDeleteShader(shaderID);
}


/** OpenGL compiles the shader source code and assign an identifier to it */
GLuint ShaderLegacy::compileShader(const std::string& source, GLuint type)
{
	// Necessary because OpenGL needs a raw string
	const char* shaderSource = source.c_str();

	GLuint shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &shaderSource, NULL);
	glCompileShader(shaderID);

	/* ERROR HANDLING */

	GLint compiled;

	// Returns the compile status from the shader and stores it into compiled
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);

	// OpenGL checks if the shader compile status is false
	if (compiled == GL_FALSE) 
	{
		// Queries the error message length of the shader
		GLint length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

		// Gets and prints the actual log
		GLchar* message = new char[length];
		glGetShaderInfoLog(shaderID, length, NULL, message);
		std::cerr << message << std::endl;
		
		// Deletes message otherwise memory leak
		delete[] message;
	}

	return shaderID;
}


void ShaderLegacy::use() const
{
	glUseProgram(_programID);
}

void ShaderLegacy::unuse() const
{
	glUseProgram(0);
}



GLint ShaderLegacy::getUniformLocation(const std::string &name) {
	return glGetUniformLocation(_programID, name.c_str());
}

void ShaderLegacy::setUniform(GLint location, const glm::vec3& vec) 
{
	glUniform3fv(location, 1, &vec[0]);
}

void ShaderLegacy::setUniform(GLint location, const glm::vec4& vec)
{
	glUniform4fv(location, 1, &vec[0]);
}


void ShaderLegacy::setUniform(std::string uniform, int i)
{
	GLuint location = glGetUniformLocation(_programID, uniform.c_str());
	glUniform1i(location, i);
}

void ShaderLegacy::setUniform(std::string uniform, const unsigned int i)
{
	GLuint location = glGetUniformLocation(_programID, uniform.c_str());
	glUniform1ui(location, i);
}

void ShaderLegacy::setUniform(std::string uniform, const float f)
{
	GLuint location = glGetUniformLocation(_programID, uniform.c_str());
	glUniform1f(location, f);
}

void ShaderLegacy::setUniform(std::string uniform, const glm::vec3& vec) 
{
	GLuint location = glGetUniformLocation(_programID, uniform.c_str());
	glUniform3fv(location, 1, &vec[0]);
}
void ShaderLegacy::setUniform(std::string uniform, const glm::mat3& mat)
{
	GLuint location = glGetUniformLocation(_programID, uniform.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
}

void ShaderLegacy::setUniform(std::string uniform, const glm::mat4& mat)
{
	GLuint location = glGetUniformLocation(_programID, uniform.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}
