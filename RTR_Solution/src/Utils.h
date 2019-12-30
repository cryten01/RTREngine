#pragma once

// STD 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <list>
#include <map> 

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW (must be after GLEW)
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

// ASSIMP
#include <assimp\scene.h>

// SOIL2
#include <SOIL2.h>

// STBI
#include <stb_image.h>

// RAPIDJSON
#include "document.h"
using namespace rapidjson;


// Custom error msg
#define EXIT_WITH_ERROR(err) \
	std::cout << "ERROR: " << err << std::endl; \
	system("PAUSE"); \
	return EXIT_FAILURE;

// Get variable name
#define STRINGIFY(Variable) (#Variable)


// Methods
namespace Utils 
{
	static std::string readInFile(const std::string& filePath)
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
}