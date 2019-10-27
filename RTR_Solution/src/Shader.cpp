#include "Shader.h"

Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
	std::string vertexShaderSource = readInFile(vertexFilePath);
	std::string fragmentShaderSource = readInFile(fragmentFilePath);
	
	GLuint vertexID = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint fragmentID = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	_programID = glCreateProgram();
	glAttachShader(_programID, vertexID);
	glAttachShader(_programID, fragmentID);
	glLinkProgram(_programID);
	glValidateProgram(_programID);

	// Delete shaders as they are linked to our program and no longer necessary
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

/** Reads in a shader file in order to provide the source code for OpenGL */
std::string Shader::readInFile(const std::string& filePath)
{
	std::string fileContent;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		fileContent.append(line + "\n");
	}

	fileStream.close();
	return fileContent;
};

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
};

void Shader::use() const
{
	glUseProgram(_programID);
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
};

void Shader::setUniform(std::string uniform, const glm::mat4& mat)
{
	GLuint location = glGetUniformLocation(_programID, uniform.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
};


Shader::~Shader()
{
	glUseProgram(0);
	glDeleteProgram(_programID);
}
