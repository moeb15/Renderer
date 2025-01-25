#include "Scenes/Scene.h"
#include "Scene.h"


namespace Yassin
{
	Scene::Scene()
	{
		m_PointLights.reserve(Yassin::MAX_LIGHT_COUNT);
	}

	void Scene::RenderScene(Window& window)
	{
		DirectX::XMMATRIX lView;
		DirectX::XMMATRIX lProj;
		DirectX::XMMATRIX lViewProj;

		m_Light->GetView(lView);
		m_Light->GetProjection(lProj);

		lViewProj = DirectX::XMMatrixMultiply(lView, lProj);

		 for(const std::unique_ptr<Renderable>& renderable : m_Renderables)
		 {
			 renderable->UpdateLightDirection({ m_Sun->GetDirection() });
			 renderable->UpdateCameraPosition({ m_Camera->GetPosition() });
			 renderable->UpdateLighting({ lViewProj, m_Light->GetPosition() },
				 { m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetSpecularColor(),
				 m_Light->GetSpecularPower() });

			 window.GetRenderer().Submit(renderable.get());
		 }

		 window.GetRenderer().Render(*m_Camera, lViewProj, m_PointLights);
	}

	void Scene::UpdateSceneShaders(std::string material)
	{
		if (material == m_SceneShaders)
		{
			if(material == "Phong Material")
			{
				m_PhongShaded = true;
				m_PBRShaded = false;
				m_BlinnPhongShaded = false;
			}
			if (material == "Blinn-Phong Material")
			{
				m_BlinnPhongShaded = true;
				m_PBRShaded = false;
				m_PhongShaded = false;
			}
			if(material == "PBR Material")
			{
				m_PBRShaded = true;
				m_PhongShaded = false;
				m_BlinnPhongShaded = false;
			}
			return;
		}
		else
		{
			if (material == "Phong Material")
			{
				m_PhongShaded = true;
				m_PBRShaded = false;
				m_BlinnPhongShaded = false;
			}
			if (material == "Blinn-Phong Material")
			{
				m_BlinnPhongShaded = true;
				m_PBRShaded = false;
				m_PhongShaded = false;
			}
			if (material == "PBR Material")
			{
				m_PBRShaded = true;
				m_PhongShaded = false;
				m_BlinnPhongShaded = false;
			}
		}
		
		m_SceneShaders = material;

		for (const std::unique_ptr<Renderable>& renderable : m_Renderables)
		{
			renderable->SetMaterial(material);
		}
	}
}