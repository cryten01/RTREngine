#include "rtrpch.h"
#include "Shader.h"

using namespace RTREngine;

/**
*	Creates a shader porgram. Needs at least one shader
**/
Shader::Shader(const std::string& vertexFileName, const std::string& fragmentFileName, const std::string& geometryFileName)
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


Shader::Shader(const std::string & computeFileName)
{
	_programID = glCreateProgram();
	addToProgram(GL_COMPUTE_SHADER, computeFileName);

	// Linking and validation
	glLinkProgram(_programID);
	glValidateProgram(_programID);
}


Shader::~Shader()
{
	glUseProgram(0);
	glDeleteProgram(_programID);
}


void Shader::addToProgram(GLenum shaderType, const std::string& fileName)
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
GLuint Shader::compileShader(const std::string& source, GLuint type)
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


void Shader::use() const
{
	glUseProgram(_programID);
}

void Shader::unuse() const
{
	glUseProgram(0);
}



GLint Shader::getUniformLocation(const std::string &name) {
	return glGetUniformLocation(_programID, name.c_str());
}

void Shader::setUniform(GLint location, const glm::vec3& vec) 
{
	glUniform3fv(location, 1, &vec[0]);
}

void Shader::setUniform(GLint location, const glm::vec4& vec)
{
	glUniform4fv(location, 1, &vec[0]);
}


void Shader::setUniform(std::string uniform, int i)
{
	GLuint location = glGetUniformLocation(_programID, uniform.c_str());
	glUniform1i(location, i);
}

void Shader::setUniform(std::string uniform, const unsigned int i)
{
	GLuint location = glGetUniformLocation(_programID, uniform.c_str());
	glUniform1ui(location, i);
}

void Shader::setUniform(std::string uniform, const float f)
{
	GLuint location = glGetUniformLocation(_programID, uniform.c_str());
	glUniform1f(location, f);
}

void Shader::setUniform(std::string uniform, const glm::vec3& vec) 
{
	GLuint location = glGetUniformLocation(_programID, uniform.c_str());
	glUniform3fv(location, 1, &vec[0]);
}
void Shader::setUniform(std::string uniform, const glm::mat3& mat)
{
	GLuint location = glGetUniformLocation(_programID, uniform.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setUniform(std::string uniform, const glm::mat4& mat)
{
	GLuint location = glGetUniformLocation(_programID, uniform.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}