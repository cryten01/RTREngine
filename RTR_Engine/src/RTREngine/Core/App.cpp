#include "rtrpch.h"
#include "RTREngine/Core/App.h"
#include "RTREngine/Core/Log.h"
#include "RTREngine/Input/Input.h"

#include <glfw/glfw3.h>


namespace RTREngine {

	App* App::s_Instance = nullptr;

	App::App()
	{
		RTR_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = Window::Create();
		//m_Window->SetEventCallback(RTR_BIND_EVENT_FN(App::OnEvent));
	}

	App::~App()
	{
	}

	void App::OnEvent(Event& e)
	{
		//EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<WindowCloseEvent>(RTR_BIND_EVENT_FN(App::OnWindowClose));
		//dispatcher.Dispatch<WindowResizeEvent>(RTR_BIND_EVENT_FN(App::OnWindowResize));
	}

	void App::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1, 0.1, 0.1, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	bool App::OnWindowClose(WindowCloseEvent& e)
	{
		return false;
	}

	bool App::OnWindowResize(WindowResizeEvent& e)
	{
		return false;
	}
}
