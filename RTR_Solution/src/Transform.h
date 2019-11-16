#pragma once
#include "Utils.h"
#include "Shader.h"

class Transform
{
private: 
	std::shared_ptr<Transform> _parent;

	glm::vec3 _localPos;
	glm::vec3 _localRot;
	glm::vec3 _localScale;
	glm::vec3 _globalPos;

	glm::mat4 _modelMatrix;			// stores relative transformation to parent
	glm::mat4 _transformMatrix;

public:
	 Transform(glm::mat4 modelMatrix, glm::mat4 transformMatrix);
	~Transform();

	// Setter
	void setParent(std::shared_ptr<Transform> parent);
	
	void setLocalPos(glm::vec3 position);
	void setLocalRot(glm::vec3 rotation);
	void setLocalScale(glm::vec3 scale);
	void setGlobalPos(glm::vec3 position);

	void setModelMatrix(glm::mat4 modelMatrix);
	void setTransformMatrix(glm::mat4 transformMatrix);

	// Getter
	glm::vec3& getGlobalPos();

	// Methods
	void updateModelMatrix();
	void resetModelMatrix();
	void transform(glm::mat4 transformation);
	void setUniforms(std::shared_ptr<Shader> shader);
};

