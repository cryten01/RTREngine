#pragma once
#include "Utils.h"
#include "Window.h"
#include "Controls.h"

class Framework
{
private:
	std::unique_ptr<Window> window;

	float currentTime;
	float lastTime;
	float deltaTime;
	float runTime;
	int frames;
	int fps;

	void update();
	void render();

public:
	 Framework();
	~Framework();

	int init();
	int destroy();
	void startRenderLoop();

	static void APIENTRY DebugCallbackDefault(GLenum source, GLenum textype, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
	static std::string FormatDebugOutput(GLenum source, GLenum textype, GLuint id, GLenum severity, const char* msg);
};

