#pragma once
#include "Utils.h"

/**
*	Note all callback functions and variables must be global or static in order to be C compatible
*	For further information see: https://gamedev.stackexchange.com/questions/58541/how-can-i-associate-a-key-callback-with-a-wrapper-class-instance
**/

namespace Controls
{
	// Mouse controls
	static double mouse_x, mouse_y;

	// Camera controls
	static bool _wireframe;
	static bool _culling;
	static bool _dragging;
	static bool _strafing;
	static float _zoom;
	static float _height;
	static bool _freezeScene;

	// Post processing controls
	static bool _hdr;
	static float _exposure;

	// Method definitions
	static void key_polling(GLFWwindow* window, float deltaTime)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			_height += 4.2f * deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			_height -= 4.2f * deltaTime;
		}
	}

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			_dragging = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			_dragging = false;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			_strafing = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
			_strafing = false;
		}
	}

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		_zoom -= float(yoffset) * 0.5f;
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		// F1 - Wireframe
		// F2 - Culling
		// F3 - HDR
		// F4 - Freeze
		// Esc - Exit

		if (action != GLFW_RELEASE) return;

		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			break;
		case GLFW_KEY_F1:
			_wireframe = !_wireframe;
			glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);
			break;
		case GLFW_KEY_F2:
			_culling = !_culling;
			if (_culling) glEnable(GL_CULL_FACE);
			else glDisable(GL_CULL_FACE);
			break;
		case GLFW_KEY_F3:
			_hdr = !_hdr;
			break;
		case GLFW_KEY_F4:
			_freezeScene = !_freezeScene;
			break;
		}
	}

	static void init(GLFWwindow* window)
	{
		// Set camera control defaults here
		_wireframe = false;
		_culling = true;
		_dragging = false;
		_strafing = false;
		_zoom = 30.0f;
		_height = 10.0f;
		_freezeScene = false;

		// Set post processing controls defaults here
		_hdr = false;
		_exposure = 1.0f;

		// Set callbacks here
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetScrollCallback(window, scroll_callback);
	}
}
