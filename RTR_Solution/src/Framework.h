#pragma once
#include "Utils.h"
#include "Window.h"
#include "Controls.h"

class Framework
{
private: 
	std::unique_ptr<Window> window;

public:
	 Framework();
	~Framework();

	int init();
	void initRenderLoop();
	int destroy();

	static void APIENTRY DebugCallbackDefault(GLenum source, GLenum textype, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
	static std::string FormatDebugOutput(GLenum source, GLenum textype, GLuint id, GLenum severity, const char* msg);
};