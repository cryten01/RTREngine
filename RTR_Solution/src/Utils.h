#pragma once

// STD 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

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

// Custom error msg
#define EXIT_WITH_ERROR(err) \
	std::cout << "ERROR: " << err << std::endl; \
	system("PAUSE"); \
	return EXIT_FAILURE;