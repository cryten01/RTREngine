#include "Camera.h"


Camera::Camera(float fov, float aspect, float near, float far)
{
	// Creates a new projection matrix
	_projMatrix = glm::perspective(
		glm::radians(fov),
		aspect,
		near,
		far
	);

	// Initial values
	_position = glm::vec3(0.0f, 0.0f, 0.0f);
	_pitch = 0.0f;
	_yaw = 0.0f;
}

glm::vec3 Camera::getPosition() 
{
	return this->_position;
};


glm::mat4 Camera::getViewProjectionMatrix() 
{
	return this->_projMatrix * _viewMatrix;
};


void Camera::update(int x, int y, float zoom, bool dragging, bool strafing) 
{
	if (dragging)
	{
		// Calculate yaw by adding relative change of mouseX
		_yaw -= (_mouseX - x);
		// Keep yaw in range between -360
		_yaw = fmodf(_yaw, 360.0f);

		// Calculate pitch by adding relative change of mouseY
		_pitch += (_mouseY - y);
		// Clamp pitch to prevent camera inversion
		_pitch = clamp(_pitch, -90.0f, 90.0f);

		std::cout << "Dragging: " << _yaw << " " << _pitch << std::endl;
	} 
	else 
	{

	}

	_mouseX = x;
	_mouseY = y;
};


float Camera::clamp(float n, float lower, float upper)
{
	return std::max(lower, std::min(n, upper));
}

Camera::~Camera()
{
}
