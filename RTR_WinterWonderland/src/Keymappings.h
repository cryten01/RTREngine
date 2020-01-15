#pragma once

#include <RTR.h>


namespace WinterWonderland 
{
	void TriggerKeyCallback() 
	{
		if (RTREngine::Input::IsKeyPressed(RTR_KEY_TAB))
			RTR_CLIENT_TRACE("Tab key is pressed!");
	}
}