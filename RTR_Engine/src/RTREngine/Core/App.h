#pragma once

#include "RTREngine/Core/Core.h"
#include "RTREngine/Core/Window.h"
#include "RTREngine/Events/Event.h"
#include "RTREngine/Events/AppEvent.h"
#include "RTREngine/Layers/Layerstack.h"


int main(int argc, char** argv);

namespace RTREngine
{
	class App
	{
	public:
		App();
		virtual ~App();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		
		inline static App& Get() { return *s_Instance; }

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static App* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined on client side
	App* CreateApp();
}
