#pragma once
#include "Utils.h"
#include "Shader.h"
#include "Transform.h"
#include "Mesh.h"

/**
* 
**/
class SceneObject
{
private:
	std::vector<std::shared_ptr<SceneObject>> _children;
	std::shared_ptr<Shader> _shader;
	std::shared_ptr<Transform> _transform;
	std::shared_ptr<Mesh> _mesh;

public:
	 SceneObject(std::shared_ptr<Shader> shader, glm::mat4 transformMatrix);
	~SceneObject();

	// Getter
	std::shared_ptr<Transform> getTransform();
	std::shared_ptr<Mesh> getMesh();

	// Setter
	void setShader(std::shared_ptr<Shader> shader);
	void setTransform(std::shared_ptr<Transform> transform);
	void setMesh(std::shared_ptr<Mesh> mesh);

	// Methods
	void addChild(std::shared_ptr<SceneObject> child);
	void update();
	void updateAll();
	void render();
	void renderAll();
};

