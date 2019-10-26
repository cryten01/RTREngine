#include "Camera.h"


Camera::Camera(float fov, float aspect, float near, float far)
{
	_projMatrix = glm::perspective(
		glm::radians(fov),
		aspect,
		near,
		far
	);

	_position = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 Camera::getPosition() 
{
	return this->_position;
};


glm::mat4 Camera::getViewProjectionMatrix() 
{
	// TODO: multiply with viewMatrix
	return this->_projMatrix;
};


void Camera::update(int x, int y, float zoom, bool dragging, bool strafing) 
{
	
};


Camera::~Camera()
{
}
