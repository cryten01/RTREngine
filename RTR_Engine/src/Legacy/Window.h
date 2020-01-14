#pragma once
#include "Utils.h"
#include "Resources.h"
#include "Scene.h"

namespace RTREngine
{
	class Scene;

	class WindowLegacy
	{
	private:
		GLFWwindow* _window;
		const int _width, _height;
		const char* _title;
		std::shared_ptr<Scene> _activeScene;

		int init();

	public:
		 WindowLegacy(const char* title, const int width, const int height);
		~WindowLegacy();

		GLFWwindow* getGLFWWindow();
		const int getWidth();
		const int getHeight();
		void setActiveScene(std::shared_ptr<Scene> scene);

		void update(float deltaTime);
		void render();
	};
}