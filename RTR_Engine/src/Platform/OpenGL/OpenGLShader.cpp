#include "rtrpch.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace RTREngine {

	/*
	*	Converts the shader type received from a shader source file into an actual type
	*/
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "compute")
			return GL_COMPUTE_SHADER;

		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "geometry")
			return GL_GEOMETRY_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		RTR_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}



	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_ShaderName = filepath.substr(lastSlash, count);
	}



	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_ShaderName(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}



	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ShaderProgID);
	}



	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string fileContent;
		std::ifstream fileStream(filepath, std::ios::in | std::ios::binary); // binary because no processing desired

		if (fileStream.is_open())
		{
			// Determine file size
			fileStream.seekg(0, std::ios::end);
			size_t fileSize = fileStream.tellg();

			// Load into string if there is content
			if (fileSize != -1)
			{
				fileContent.resize(fileSize);
				fileStream.seekg(0, std::ios::beg);
				fileStream.read(&fileContent[0], fileSize);
				fileStream.close();
			}
			else
			{
				RTR_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			RTR_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return fileContent;
	}


	/*
	*	Splits up a single shader file into multiple shader files (e.g. vertex, geomtery, fragment)
	*/
	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);

		// Find start of shader type declaration line
		size_t pos = source.find(typeToken, 0); 

		while (pos != std::string::npos)
		{
			// Find end of shader type declaration line (= first occurence of new line or carriage return)
			size_t endOfLine = source.find_first_of("\r\n", pos);
			RTR_CORE_ASSERT(endOfLine != std::string::npos, "Syntax error");

			//Start of shader type name (after "#type " keyword)
			size_t begin = pos + typeTokenLength + 1; 
			std::string type = source.substr(begin, endOfLine - begin);
			RTR_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			//Start of shader code after shader type declaration line
			size_t nextLinePos = source.find_first_not_of("\r\n", endOfLine); 
			RTR_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");

			//Start of next shader type declaration line
			pos = source.find(typeToken, nextLinePos); 

			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}


	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		RTR_CORE_ASSERT(shaderSources.size() <= 3, "RTREngine is supporting only 3 shaders in one file at most!");

		std::vector<GLenum> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& shaderSource : shaderSources)
		{
			GLenum type = shaderSource.first;
			const std::string& source = shaderSource.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			/* ERROR HANDLING */

			GLint isCompiled = 0;

			// Returns the compile status from the shader and stores it into isCompiled
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			// OpenGL checks if the shader compile status is false
			if (isCompiled == GL_FALSE)
			{
				// Queries the error message length of the shader
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// Gets and prints the actual log
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);


				glDeleteShader(shader);

				RTR_CORE_ERROR("{0}", infoLog.data());
				RTR_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		m_ShaderProgID = program;

		// Link our program
		glLinkProgram(program);


		/* ERROR HANDLING */

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			RTR_CORE_ERROR("{0}", infoLog.data());
			RTR_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
	}


	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ShaderProgID);
	}


	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}


	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_ShaderProgID, name.c_str());
		glUniform1i(location, value);
	}


	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_ShaderProgID, name.c_str());
		glUniform1f(location, value);
	}


	void OpenGLShader::SetVec2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_ShaderProgID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}


	void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_ShaderProgID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}


	void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_ShaderProgID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}


	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_ShaderProgID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}


	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_ShaderProgID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}