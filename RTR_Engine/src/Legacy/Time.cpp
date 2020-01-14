#include "rtrpch.h"
#include "Time.h"

using namespace RTREngine;

TimeLegacy::TimeLegacy()
{
	currentTime = float(glfwGetTime());
	lastTime = float(glfwGetTime());
	deltaTime = 0.0f;
	runTime = 0.0f;
	frames = 0;
	fps = 0;
}


TimeLegacy::~TimeLegacy()
{
}

float TimeLegacy::getDeltaTime() 
{
	return this->deltaTime;
}

void TimeLegacy::update()
{
	// Update time
	deltaTime = currentTime;
	currentTime = float(glfwGetTime());
	deltaTime = currentTime - deltaTime;
	runTime += deltaTime;

	// Update FPS counter
	frames++;
	if (currentTime - lastTime >= 1.0) {
		fps = frames;
		frames = 0;
		lastTime += 1.0;
	}
}
