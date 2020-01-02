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
		std::unique_ptr<Window> window;
		std::unique_ptr<Input> input;
		std::unique_ptr<Time> time;

		int init();
		int destroy();
		void startRenderLoop();

	public:
		 Framework();
		~Framework();

		static void APIENTRY DebugCallbackDefault(GLenum source, GLenum textype, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
		static std::string FormatDebugOutput(GLenum source, GLenum textype, GLuint id, GLenum severity, const char* msg);
	};
}

