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
	// Update _yaw and _pitch if dragging is activated
	if (dragging)
	{
		// Calculate yaw by adding relative change of mouseX
		_yaw += (_mouseX - x);
		
		// Keep yaw in range between 0-360
		_yaw = fmodf(_yaw, 360.0f);
		_yaw = _yaw < 0.0f ? _yaw + 360.0f : _yaw;

		// Calculate pitch by adding relative change of mouseY
		_pitch += (_mouseY - y);

		// Clamp pitch to prevent camera inversion
		_pitch = clamp(_pitch, -90.0f, 90.0f);
	} 

	/* Viewmatrix transformations */

	// reset _viewMatrix for calculating absolute values 
	_viewMatrix = glm::mat4(1.0f); 
	
	// apply _yaw values to _viewMatrix (y axis)
	_viewMatrix = glm::rotate(_viewMatrix, glm::radians(_yaw), glm::vec3(0.0, 1.0, 0.0));
	
	// apply _pitch values to _viewMatrix (x axis)
	_viewMatrix = glm::rotate(_viewMatrix, glm::radians(_pitch), glm::vec3(1.0, 0.0, 0.0));
	
	// set distance based on radius 
	float radius = 8.0f;
	_viewMatrix = glm::translate(_viewMatrix, glm::vec3(0, 0, radius)); // (y axis for debugging purposes!)
	
	// invert direction in order to view at origin
	_viewMatrix = inverse(_viewMatrix);

	// Updates current mouse positions
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
