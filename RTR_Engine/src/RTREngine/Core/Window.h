#pragma once

#include "rtrpch.h"

#include "RTREngine/Core/Core.h"
#include "RTREngine/Events/Event.h"

namespace RTREngine {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "RTR Engine", unsigned int width = 640, unsigned int height = 480)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class RTR_API Window
	{
	public:
		// Events are dispatched to callback function
		using EventCallbackFn = std::function<void(Event&)>; 

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};

}