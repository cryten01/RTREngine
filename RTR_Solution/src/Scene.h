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
		std::shared_ptr<Camera> _camera;

		std::vector<std::shared_ptr<SceneObject>> _sceneObjects;
		std::vector<std::shared_ptr<Renderable>> _renderObjects;

	public:
		Scene();
		~Scene();

		std::shared_ptr<ParticleSystem> _particles; // For debugging only

		void setActiveSkybox(std::shared_ptr<Skybox> skybox);
		void setActiveCamera(std::shared_ptr<Camera> camera);

		void addSceneObject(std::shared_ptr<SceneObject> sceneObject);
		void addRenderable(std::shared_ptr<Renderable> renderObject);

		void update(float deltaTime);
		void setPerFrameUniforms(std::shared_ptr<Shader> shader);
		void render(std::shared_ptr<Shader> shader);
	};
}