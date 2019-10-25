#pragma once

// STD 
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL\glew.h>

// GLFW (must be after GLEW)
#include <GLFW/glfw3.h>

// Custom error msg
#define EXIT_WITH_ERROR(err) \
	std::cout << "ERROR: " << err << std::endl; \
	system("PAUSE"); \
	return EXIT_FAILURE;