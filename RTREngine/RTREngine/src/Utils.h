#pragma once

// STD 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


// GLEW
#define GLEW_STATIC
#include <GL\glew.h>

// GLFW (must be after GLEW)
#include <GLFW/glfw3.h>

// GLM
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>


// Custom error msg
#define EXIT_WITH_ERROR(err) \
	std::cout << "ERROR: " << err << std::endl; \
	system("PAUSE"); \
	return EXIT_FAILURE;