#pragma once
#include "Utils.h"
#include "Scene.h"

namespace RTREngine
{
	class Window
	{
	private:
		GLFWwindow* _window;
		const int _WIDTH;
		const int _HEIGHT;
		const char* _TITLE;
		std::shared_ptr<Scene> _activeScene;

		int init();

	public:
		Window(const char* title, const int width, const int height);
		~Window();

		void setActiveScene(std::shared_ptr<Scene> scene);
		GLFWwindow* getGLFWWindow();
		const int getWidth();
		const int getHeight();
		void update(float deltaTime);
		void render();
	};
}