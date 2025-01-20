#include "Scenes/SponzaScene.h"
#include "SponzaScene.h"

namespace Yassin 
{
	void SponzaScene::Init(Camera* camera, DirectionalLight* sun, PointLight* light)
	{
		m_PhongShaded = true;
		m_PBRShaded = false;
		m_SceneShaders = "Phong Material";

		m_Camera = camera;
		m_Sun = sun;
		m_Light = light;
		
		DirectX::XMMATRIX world = DirectX::XMMatrixScaling(0.25f, 0.25f, 0.25f);
		std::unique_ptr<Model>sponza = std::make_unique<Model>(m_SceneShaders, "src/Assets/Models/Sponza_GLTF/Sponza.gltf", world);

		m_Renderables.push_back(std::move(sponza));
	}
}