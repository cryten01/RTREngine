#pragma once
#include "Utils.h"
#include "Shader.h"
#include "Transform.h"
#include "Mesh.h"
#include "Light.h"

/**
* 
**/
class SceneObject
{
private:
	std::vector<std::shared_ptr<SceneObject>> _children;
	std::shared_ptr<Shader> _shader;
	std::shared_ptr<Transform> _transform;
	std::vector<std::shared_ptr<Mesh>> _meshes;	// subMeshes of a scene object node
	std::shared_ptr<SpotLight> _light;

public:
	 SceneObject(std::shared_ptr<Shader> shader, glm::mat4 modelMatrix);
	~SceneObject();

	// Getter
	std::shared_ptr<Transform> getTransform();
	std::shared_ptr<Shader> getShader();
	std::shared_ptr<Mesh> getMeshAt(int index);

	// Setter
	void setLight(std::shared_ptr<SpotLight> pointLight);
	void setShader(std::shared_ptr<Shader> shader);
	void setTransform(std::shared_ptr<Transform> transform);

	// Methods
	void addMesh(std::shared_ptr<Mesh> mesh);
	void addChild(std::shared_ptr<SceneObject> child);

	void update();
	void updateAll();
	void render();
	void renderAll();
};

