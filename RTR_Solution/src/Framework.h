#pragma once
#include "Utils.h"
#include "Window.h"
#include "Input.h"
#include "Time.h"

namespace RTREngine
{
	class Framework
	{
	private:
		Framework();
		~Framework();

		std::shared_ptr<Window> window;
		std::unique_ptr<Input> input;
		std::unique_ptr<Time> time;

	public:
		static Framework& Instance() {
			static Framework framework;
			return framework;
		};

		int init();
		int destroy();
		void startRenderLoop();
		std::shared_ptr<Window> getWindow();

		static void APIENTRY DebugCallbackDefault(GLenum source, GLenum textype, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
		static std::string FormatDebugOutput(GLenum source, GLenum textype, GLuint id, GLenum severity, const char* msg);
	};
}