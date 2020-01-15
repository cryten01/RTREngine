#pragma once

#include "RTREngine/Core/Core.h"
#include "RTREngine/Core/Window.h"
#include "RTREngine/Core/Time.h"
#include "RTREngine/Events/Event.h"
#include "RTREngine/Events/AppEvent.h"
#include "RTREngine/Layers/Layerstack.h"
#include "RTREngine/ImGui/ImGuiLayer.h"


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
		inline static App& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;

		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		static App* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined on client side
	App* CreateApp();
}
