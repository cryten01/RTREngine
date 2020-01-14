#pragma once
#include "Utils.h"

namespace RTREngine 
{
	class TimeLegacy
	{
	private:
		float currentTime;
		float lastTime;
		float deltaTime;
		float runTime;
		int frames;
		int fps;

	public:
		TimeLegacy();
		~TimeLegacy();

		float getDeltaTime();
		void update();
	};
}
