#pragma once
#include "Utils.h"
#include "Skybox.h"
#include "SceneObject.h"
#include "Resources.h"

namespace RTREngine
{
	class Scene
	{
	private:
		std::shared_ptr<Skybox> _skybox;
		std::vector<std::shared_ptr<SceneObject>> _sceneObjects;

	public:
		Scene();
		~Scene();

		void setActiveSkybox(std::shared_ptr<Skybox> skybox);
		void addSceneObject(std::shared_ptr<SceneObject> sceneObject);

		void update(float deltaTime);
		void render(std::shared_ptr<Shader> shader);
	};
}