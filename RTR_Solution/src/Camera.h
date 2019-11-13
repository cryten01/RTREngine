#pragma once

#include "Utils.h"
#include "SceneComponent.h"

class Camera : public SceneComponent
{
private:
	glm::mat4 _viewMatrix;
	glm::mat4 _projMatrix;
	int _mouseX, _mouseY;	// safes the last known positions
	float _yaw, _pitch;
	glm::vec3 _position;
	glm::vec3 _strafe;

	void processMouseMovement(int x, int y);


public:
	 Camera(float fov, float aspect, float near, float far);
	~Camera();

	glm::vec3 getPosition();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjMatrix();

	void update(int x, int y, float zoom, bool dragging, bool strafing);
	float clamp(float n, float lower, float upper);

};

