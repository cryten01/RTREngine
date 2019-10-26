#pragma once

#include "Utils.h"

class Camera
{
private:
	glm::mat4 _viewMatrix;
	glm::mat4 _projMatrix;
	int _mouseX, _mouseY;
	float _yaw, _pitch;
	glm::vec3 _position;
	glm::vec3 _strafe;


public:
	 Camera(float fov, float aspect, float near, float far);
	~Camera();

	glm::vec3 getPosition();
	glm::mat4 getViewProjectionMatrix();

	void update(int x, int y, float zoom, bool dragging, bool strafing);
};

