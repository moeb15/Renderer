#include "Scenes/TestScene.h"

namespace Yassin
{
	void TestScene::Init(Camera* camera, DirectionalLight* sun, PointLight* light)
	{
		m_PhongShaded = true;
		m_PBRShaded = false;
		m_BlinnPhongShaded = false;
		m_SceneShaders = "Phong Material";

		m_Camera = camera;
		m_Sun = sun;
		m_Light = light;

		for (int i = 0; i < Yassin::MAX_LIGHT_COUNT; i++)
		{
			if (i >= Yassin::MAX_LIGHT_COUNT) break;

			PointLight newLight = PointLight(90.f, 1.f, 1.f, 100.f, 10.0f);
			float x, z;
			x = (float)(i % 8);
			z = 10.f * sinf((float)i);
			newLight.SetPosition(x, 1.f + (i % 10) * (float)pow(-1.f, i), z);
			float r, g, b;
			r = (float)abs(sin(i));
			g = (float)abs(cos(i));
			b = (float)abs(sin(i) * cos(i));
			newLight.SetAmbientColor(r, g, b, 1.0f);
			m_PointLights.push_back(newLight);
		}

		DirectX::XMMATRIX world = DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f);
		std::unique_ptr<Model> nanosuit = std::make_unique<Model>(m_SceneShaders, "src/Assets/Models/Nanosuit/nanosuit.obj", world);

		m_Renderables.push_back(std::move(nanosuit));
	}
}