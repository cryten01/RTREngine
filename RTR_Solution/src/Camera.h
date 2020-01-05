#pragma once
#include "Utils.h"
#include "Interfaces.h"
#include "Shader.h"
#include "Input.h"

namespace RTREngine
{
	class Camera : public SceneComponent, Uniformable
	{
	private:
		std::shared_ptr<Input> _input;

		glm::mat4 _viewMatrix;
		glm::mat4 _projMatrix;
		int _mouseX, _mouseY;	// safes the last known positions
		float _yaw, _pitch;
		glm::vec3 _position;
		glm::vec3 _strafe;

		void processMouseMovement(int x, int y);

	public:
		 Camera(float fov, float aspect, float near, float far, std::shared_ptr<Input> input);
		~Camera();

		glm::vec3 getPosition();
		glm::mat4 getViewMatrix();
		glm::mat4 getProjMatrix();

		void setUniforms(std::shared_ptr<Shader> shader);
		void update(float deltaTime) override;
	};
}
