#include "Scenes/SponzaScene.h"
#include "SponzaScene.h"

namespace Yassin 
{
	void SponzaScene::Init(Camera* camera, DirectionalLight* sun, PointLight* light)
	{
		m_PhongShaded = true;
		m_PBRShaded = false;
		m_BlinnPhongShaded = false;
		m_SceneShaders = "Phong Material";

		m_Camera = camera;
		m_Sun = sun;
		m_Light = light;
		
		for (int i = 0; i < 64; i++)
		{
			PointLight newLight = PointLight(90.f, 1.f, 1.f, 100.f);
			float x, z;
			x = (float)(i % 8);
			z = 10.f * sinf((float)i);
			newLight.SetPosition(x, 5.f, z);
			m_PointLights.push_back(newLight);
		}

		DirectX::XMMATRIX world = DirectX::XMMatrixScaling(0.25f, 0.25f, 0.25f);
		std::unique_ptr<Model>sponza = std::make_unique<Model>(m_SceneShaders, "src/Assets/Models/Sponza_GLTF/Sponza.gltf", world);

		m_Renderables.push_back(std::move(sponza));
	}
}