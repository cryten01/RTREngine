#pragma once

// STD 
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
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

// SOIL2
#include <SOIL2.h>

// Custom error msg
#define EXIT_WITH_ERROR(err) \
	std::cout << "ERROR: " << err << std::endl; \
	system("PAUSE"); \
	return EXIT_FAILURE;