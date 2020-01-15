#pragma once

#include "RTREngine/Layers/Layer.h"
#include "RTREngine/Events/AppEvent.h"
#include "RTREngine/Events/KeyEvent.h"
#include "RTREngine/Events/MouseEvent.h"

namespace RTREngine {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}