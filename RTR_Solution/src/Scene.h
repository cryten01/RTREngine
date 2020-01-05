#pragma once
#include "Utils.h"
#include "Skybox.h"
#include "SceneObject.h"
#include "Resources.h"

namespace RTREngine
{
	class Renderable;
	class Skybox;

	class Scene
	{
	private:
		std::shared_ptr<Skybox> _skybox;
		std::vector<std::shared_ptr<SceneObject>> _sceneObjects;
		std::vector<std::shared_ptr<Renderable>> _renderObjects;

	public:
		Scene();
		~Scene();

		std::shared_ptr<ParticleSystem> _particles; // For debugging only

		void setActiveSkybox(std::shared_ptr<Skybox> skybox);
		void addSceneObject(std::shared_ptr<SceneObject> sceneObject);
		void addRenderable(std::shared_ptr<Renderable> renderObject);

		void update(float deltaTime);
		void setUniforms(std::shared_ptr<Shader> shader);
		void render(std::shared_ptr<Shader> shader);
	};
}