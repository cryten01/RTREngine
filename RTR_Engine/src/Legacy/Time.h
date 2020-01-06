#pragma once
#include "Utils.h"

namespace RTREngine 
{
	class Time
	{
	private:
		float currentTime;
		float lastTime;
		float deltaTime;
		float runTime;
		int frames;
		int fps;

	public:
		Time();
		~Time();

		float getDeltaTime();
		void update();
	};
}
