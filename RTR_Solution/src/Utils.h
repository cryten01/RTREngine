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



/* --------------------------------------------- */
// Meta programming
/* --------------------------------------------- */

/** GOAL for identical types (tunneling): 
	shader->setUniform(pointLights[i].color)	// Array
	shader->setUniform(material.alpha)			// Single variable

	GOAL for non-identical types (location based tunneling):
	shader->setUniform(location = 0)			// GPU only dataTypes
*/


// Gets variable name

// Check if uniform does exist (once at compile time, debug only)

// Check if declaration/definition name is identical  (once at compile time, debug only)

// Check if structure is identical (once at compile time, debug only)

// Set all struct attributes (ensures access to all attributes on GPU side)