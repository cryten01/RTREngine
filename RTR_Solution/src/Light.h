/*
* Copyright 2017 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/
#pragma once
#include "Utils.h"

struct DirectionalLight {
	DirectionalLight() {
		enabled = false;
	}

	DirectionalLight(glm::vec3 color, glm::vec3 direction, bool enabled = true)
		: color(color), direction(glm::normalize(direction)), enabled(enabled)
	{}

	bool enabled;
	glm::vec3 color;
	glm::vec3 direction;


	void setUniforms(std::shared_ptr<Shader> shader)
	{
		shader->setUniform("dirL.color", this->color);
		shader->setUniform("dirL.direction", this->direction);
	}
};


struct PointLight {
	PointLight() {
		enabled = false;
	}

	PointLight(glm::vec3 color, glm::vec3 position, glm::vec3 attenuation, bool enabled = true)
		: color(color), position(position), attenuation(attenuation), enabled(enabled)
	{}

	bool enabled;
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 attenuation; // x = constant, y = linear, z = quadratic 


	// Index is necessary because light is stored in an array in shader file
	void setUniforms(std::shared_ptr<Shader> shader, unsigned int index)
	{
		shader->setUniform("pointL[" + std::to_string(index) + "].color", this->color);
		shader->setUniform("pointL[" + std::to_string(index) + "].position", this->position);
		shader->setUniform("pointL[" + std::to_string(index) + "].attenuation", this->attenuation);
	}
};


struct SpotLight {
	SpotLight() {
		enabled = false;
	}

	SpotLight(glm::vec3 color, glm::vec3 position, glm::vec3 attenuation, glm::vec3 direction, float innerAngle, float outerAngle, bool enabled = true)
		: color(color), position(position), attenuation(attenuation), direction(direction), innerAngle(innerAngle), outerAngle(outerAngle), enabled(enabled)
	{}

	bool enabled;
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 attenuation; // x = constant, y = linear, z = quadratic 
	glm::vec3 direction;
	float innerAngle;
	float outerAngle;


	// Index is necessary because light is stored in an array in shader file
	void setUniforms(std::shared_ptr<Shader> shader, unsigned int index)
	{
		shader->setUniform("spotL[" + std::to_string(index) + "].color", this->color);
		shader->setUniform("spotL[" + std::to_string(index) + "].position", this->position);
		shader->setUniform("spotL[" + std::to_string(index) + "].attenuation", this->attenuation);
		shader->setUniform("spotL[" + std::to_string(index) + "].direction", this->direction);
		shader->setUniform("spotL[" + std::to_string(index) + "].innerAngle", this->innerAngle);
		shader->setUniform("spotL[" + std::to_string(index) + "].outerAngle", this->outerAngle);
	}
};