#pragma once
#include "RTREngine/Core/Core.h"

namespace RTREngine 
{
	class RTR_API App
	{
	public:
		App();
		virtual ~App();

		void Run();
	};

	// To be defined on client side
	App* CreateApp();
}
