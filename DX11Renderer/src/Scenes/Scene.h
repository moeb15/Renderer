#pragma once
#include "Renderable/RenderablePrimitives.h"
#include "Core/Window.h"

namespace Yassin
{
	class Scene
	{
	public:
		Scene();
		virtual void Init(Camera* camera, DirectionalLight* sun, PointLight* light) = 0;
		void RenderScene(Window& window);
		inline bool& GetPhongShaded() { return m_PhongShaded; }
		inline bool& GetBlinnPhongShaded() { return m_BlinnPhongShaded; }
		inline bool& GetPBRShaded() { return m_PBRShaded; }
		void UpdateSceneShaders(std::string material);

	protected:
		std::vector<std::unique_ptr<Renderable>> m_Renderables;
		DirectionalLight* m_Sun;
		PointLight* m_Light;
		Camera* m_Camera;
		bool m_BlinnPhongShaded;
		bool m_PhongShaded;
		bool m_PBRShaded;
		std::string m_SceneShaders;
		std::vector<PointLight> m_PointLights;

	};
}

