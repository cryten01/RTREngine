#include "Shader.h"

Shader::Shader(std::string vertexFilePath, std::string fragmentFilePath)
{
	std::string vertexShaderSource = readInFile(vertexFilePath);
	std::string fragmentShaderSource = readInFile(fragmentFilePath);
	
	GLuint vertexID = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint fragmentID = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	_programID = glCreateProgram();
	glAttachShader(_programID, vertexID);
	glAttachShader(_programID, fragmentID);
	glLinkProgram(_programID);

	// Checks if linking of shaders was successful
	GLint succededProgram;
	glGetProgramiv(_programID, GL_LINK_STATUS, &succededProgram);
}


std::string Shader::readInFile(const std::string& filePath)
{
	std::ifstream fileStream(filePath, std::ios::in);
	std::string fileContent;

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


GLuint Shader::compileShader(const std::string& source, GLuint type)
{
	const char* shaderSource = source.c_str();

	GLuint shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &shaderSource, NULL);
	glCompileShader(shaderID);

	// error handling
	GLint compiled;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);

	if (compiled == GL_FALSE) {

		GLint length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

		GLchar* message = new char[length];
		glGetShaderInfoLog(shaderID, length, NULL, message);

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

void Shader::setUniform(GLint location, const glm::vec4& vec)
{
	glUniform4fv(location, 1, &vec[0]);
}

void Shader::setUniform(std::string uniform, const float f)
{
	GLuint location = glGetUniformLocation(_programID, uniform.c_str());
	glUniform1f(location, f);
}

void Shader::setUniform(std::string uniform, const unsigned int i)
{
	GLuint location = glGetUniformLocation(_programID, uniform.c_str());
	glUniform1ui(location, i);
}

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
