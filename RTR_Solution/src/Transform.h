#pragma once
#include "Utils.h"
#include "Interfaces.h"
#include "Shader.h"

namespace RTREngine 
{
	class Transform : public SceneComponent
	{
	private:
		std::shared_ptr<Transform> _parent;

		glm::vec3 _localPos;	// relative pos in local space 
		glm::vec3 _localRot;	// relative rot in local space (euler angles)
		glm::vec3 _localScale;	// relative scale in local space 
		glm::vec3 _worldPos;	// absolute (= local space + parent world space) pos in world space
		glm::vec3 _right;
		glm::vec3 _up;
		glm::vec3 _forward;

		glm::mat4 _modelMatrix;		// transforms local model coordinates into world coordinates (stores relative transformation to parent)
		glm::mat4 _transformMatrix;

		void applyLocalTRS();	// applies TRS to modelMatrix (mat[col][row])
		void applyGlobalTRS();

		glm::vec3 extractPos(glm::mat4 modelMatrix);
		glm::vec3 extractRot(glm::mat4 modelMatrix);
		glm::vec3 extractScale(glm::mat4 modelMatrix);

	public:
		Transform(glm::mat4 modelMatrix);
		~Transform();

		// Setter
		void setParent(std::shared_ptr<Transform> parent);
		void setLocalPos(glm::vec3 position);
		void setLocalRot(glm::vec3 rotation);
		void setLocalScale(glm::vec3 scale);
		void setModelMatrix(glm::mat4 modelMatrix);
		void setTransformMatrix(glm::mat4 transformMatrix);

		// Getter
		glm::vec3& getLocalPos();
		glm::vec3& getLocalRot();
		glm::vec3& getDirection();

		// Methods
		void resetModelMatrix();
		void transform(glm::mat4 transformation);

		void setUniforms(std::shared_ptr<Shader> shader) override;
		void update(float deltaTime) override;
	};
}