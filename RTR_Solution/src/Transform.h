#pragma once
#include "Utils.h"
#include "Shader.h"

class Transform
{
private: 
	std::shared_ptr<Transform> _parent;

	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;

	glm::mat4 _modelMatrix;			// stores relative transformation to parent
	glm::mat4 _transformMatrix;

public:
	 Transform(glm::mat4 modelMatrix, glm::mat4 transformMatrix);
	~Transform();

	// Setter
	void setParent(std::shared_ptr<Transform> parent);
	
	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);

	void setModelMatrix(glm::mat4 modelMatrix);
	void setTransformMatrix(glm::mat4 transformMatrix);


	// Methods
	void updateModelMatrix();
	void resetModelMatrix();
	void transform(glm::mat4 transformation);
	void setUniforms(std::shared_ptr<Shader> shader);
};

