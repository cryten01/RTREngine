#pragma once
#include "Utils.h"
#include "Skybox.h"
#include "Light.h"
#include "SceneObject.h"
#include "Resources.h"

namespace RTREngine
{
	class Renderable;
	class Skybox;
	class DirectionalLight;

	class Scene
	{
	private:
		std::shared_ptr<Skybox> _skybox;
		std::shared_ptr<Camera> _camera;
		std::shared_ptr<DirectionalLight> _dirlight;

		std::vector<std::shared_ptr<SceneObject>> _sceneObjects;
		std::vector<std::shared_ptr<Renderable>> _renderObjects;

	public:
		Scene();
		~Scene();

		void setActiveSkybox(std::shared_ptr<Skybox> skybox);
		void setActiveCamera(std::shared_ptr<Camera> camera);
		void setActiveDirLight(std::shared_ptr<DirectionalLight> dirlight);

		void addSceneObject(std::shared_ptr<SceneObject> sceneObject);
		void addRenderable(std::shared_ptr<Renderable> renderObject);

		void update(float deltaTime);
		void setPerFrameUniforms(std::shared_ptr<ShaderLegacy> shader);
		void render(std::shared_ptr<ShaderLegacy> shader);
	};
}