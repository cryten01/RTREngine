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
};