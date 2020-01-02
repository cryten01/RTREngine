#pragma once
#include "Utils.h"


class Window
{
private:
	GLFWwindow* _window;

	const int WIDTH;
	const int HEIGHT;
	const char* TITLE;

	int create();

public:
	 Window(const char* title, const int width, const int height);
	~Window();

	GLFWwindow* getGLFWWindow();
	const int getWidth();
	const int getHeight();
};
