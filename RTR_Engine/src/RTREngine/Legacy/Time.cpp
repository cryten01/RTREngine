#include "rtrpch.h"
#include "Time.h"

using namespace RTREngine;

Time::Time()
{
	currentTime = float(glfwGetTime());
	lastTime = float(glfwGetTime());
	deltaTime = 0.0f;
	runTime = 0.0f;
	frames = 0;
	fps = 0;
}


Time::~Time()
{
}

float Time::getDeltaTime() 
{
	return this->deltaTime;
}

void Time::update()
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
