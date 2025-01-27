#include "Application.h"
#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"

namespace Yassin
{
	Application::Application(const ApplicationSpecification& appSpec) :
		m_Window(appSpec.width, appSpec.height, appSpec.name),
		m_CameraController(75.f, (float)appSpec.width / (float)appSpec.height)
	{
		m_Window.Init();

		sun = std::make_unique<DirectionalLight>(10.f, -10.f, 10.f);
		sun->SetPosition(1.0f, 5.f, 0.0f);

		light = std::make_unique<PointLight>(90.f, 1.f, 1.f, 100.f);
		light->SetAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
		light->SetPosition(1.0f, 4.f, 0.f);
		light->SetLookAt(0.0f, 0.0f, 0.0f);

		m_SponzaScene = std::make_unique<SponzaScene>();
		m_SponzaScene->Init(&m_CameraController.GetCamera(), sun.get(), light.get());

		m_TestScene = std::make_unique<TestScene>();
		m_TestScene->Init(&m_CameraController.GetCamera(), sun.get(), light.get());

		m_ActiveScene = m_SponzaScene.get();

		RendererContext::GetGPUInfo(m_GPUName, m_GPUMem);
	}
	
	Application::~Application()
	{
	}
	
	int Application::Run()
	{
		while(true)
		{
			if(const auto eCode = Window::ProcessMessages())
			{
				return *eCode;
			}
			Frame();
		}
	}

	void Application::Frame()
	{
		float dt = m_Timer.Tick();
		m_CameraController.OnUpdate(dt);

		m_Window.GetRenderer().BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		bool static showDemo = true;

		SettingsGui();

		m_ActiveScene->RenderScene(m_Window, m_Timer);

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		m_Window.GetRenderer().EndScene();
	}

	void Application::SettingsGui()
	{
		DirectX::XMFLOAT3 lightPosition = sun->GetPosition();
		DirectX::XMFLOAT3 lightLookAt = light->GetLookAt();
		DirectX::XMFLOAT3 lightDir = sun->GetDirection();
		DirectX::XMFLOAT4 lightSpecular = light->GetSpecularColor();
		float lightSpecularPower = light->GetSpecularPower();

		static const char* items[] = { "Forward", "Deferred" };
		static int SelectedItem = 0;

		if (ImGui::Begin("Settings"))
		{
			ImGui::Text("Scene Data");
			ImGui::Dummy(ImVec2(0.0f, 1.0f));
			ImGui::Text("Meshes Rendered : %i", m_Window.GetRenderer().GetMeshesRendered());
			ImGui::Text("Total Meshes : %i", m_Window.GetRenderer().GetTotalMeshes());

			ImGui::Checkbox("Phong Shading", &m_ActiveScene->GetPhongShaded());
			if(m_ActiveScene->GetPhongShaded())
			{
				m_ActiveScene->UpdateSceneShaders("Phong Material");
			}

			ImGui::Checkbox("Blinn-Phong Shading", &m_ActiveScene->GetBlinnPhongShaded());
			if (m_ActiveScene->GetBlinnPhongShaded())
			{
				m_ActiveScene->UpdateSceneShaders("Blinn-Phong Material");
			}
			
			ImGui::Checkbox("PBR Shading", &m_ActiveScene->GetPBRShaded());
			if(m_ActiveScene->GetPBRShaded())
			{
				m_ActiveScene->UpdateSceneShaders("PBR Material");
			}

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::Combo("Rendering Pipeline", &SelectedItem, items, std::size(items));
			
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::Text("Post-Processing Settings");
			ImGui::Dummy(ImVec2(0.0f, 1.0f));
			ImGui::Checkbox("Toggle Post-Processing", &m_Window.GetRenderer().PostProcessingEnabled());

			if (m_Window.GetRenderer().PostProcessingEnabled())
			{
				ImGui::Text("Blur");
				ImGui::Dummy(ImVec2(0.0f, 1.0f));
				ImGui::Checkbox("Gaussian Blur", &m_Window.GetRenderer().GaussianBlurEnabled());
				ImGui::Checkbox("Box Blur", &m_Window.GetRenderer().BoxBlurEnabled());

				ImGui::Dummy(ImVec2(0.0f, 2.0f));
				ImGui::Text("Anti-Aliasing");
				ImGui::Checkbox("FXAA", &m_Window.GetRenderer().FXAAEnabled());

				ImGui::Dummy(ImVec2(0.0f, 2.0f));
				ImGui::Text("Ambient Occlusion");
				ImGui::Checkbox("SSAO", &m_Window.GetRenderer().SSAOEnabled());
			}

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::Text("Directional Light Settings");
			ImGui::Dummy(ImVec2(0.0f, 1.0f));
			ImGui::SliderFloat("X-Dir", &lightDir.x, -1.f, 1.f, "%.1f");
			ImGui::SliderFloat("Y-Dir", &lightDir.y, -1.f, 1.f, "%.1f");
			ImGui::SliderFloat("Z-Dir", &lightDir.z, -1.f, 1.f, "%.1f");

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::Text("Specular Light Settings");
			ImGui::Dummy(ImVec2(0.0f, 1.0f));
			ImGui::SliderFloat("R", &lightSpecular.x, 0.f, 1.f, "%.1f");
			ImGui::SliderFloat("G", &lightSpecular.y, 0.f, 1.f, "%.1f");
			ImGui::SliderFloat("B", &lightSpecular.z, 0.f, 1.f, "%.1f");
			ImGui::SliderFloat("Specular Power", &lightSpecularPower, 0.0f, 200.f, "%.1f");

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::Text("Point Light Settings");
			ImGui::Text("Light Position");
			ImGui::Dummy(ImVec2(0.0f, 1.0f));
			ImGui::SliderFloat("X", &lightPosition.x, -50.f, 50.f, "%.1f");
			ImGui::SliderFloat("Y", &lightPosition.y, -50.f, 50.f, "%.1f");
			ImGui::SliderFloat("Z", &lightPosition.z, -50.f, 50.f, "%.1f");

			ImGui::Text("Light LookAt");
			ImGui::Dummy(ImVec2(0.0f, 1.0f));
			ImGui::SliderFloat("L_X", &lightLookAt.x, -50.f, 50.f, "%.1f");
			ImGui::SliderFloat("L_Y", &lightLookAt.y, -50.f, 50.f, "%.1f");
			ImGui::SliderFloat("L_Z", &lightLookAt.z, -50.f, 50.f, "%.1f");

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::Text("Statistics");
			ImGui::Dummy(ImVec2(0.0f, 1.0f));
			ImGui::Text("FPS : %f", ImGui::GetIO().Framerate);
			ImGui::Text("Renderer : %s", m_GPUName);
			ImGui::Text("VRAM : %i", m_GPUMem);
		}
		ImGui::End();

		//light->SetPosition(lightPosition.x, lightPosition.y, lightPosition.z);
		light->SetLookAt(lightLookAt.x, lightLookAt.y, lightLookAt.z);
		light->SetSpecularColor(lightSpecular.x, lightSpecular.y, lightSpecular.z, 1.0f);
		light->SetSpecularPower(lightSpecularPower);
		
		DirectX::XMVECTOR normalDir = DirectX::XMLoadFloat3(&lightDir);
		normalDir = DirectX::XMVector3Normalize(normalDir);
		DirectX::XMStoreFloat3(&lightDir, normalDir);
		sun->SetDirection(lightDir.x, lightDir.y, lightDir.z);
		sun->SetPosition(lightPosition.x, lightPosition.y, lightPosition.z);

		switch(SelectedItem)
		{
		case 0:
			m_Window.GetRenderer().DisableDeferred();
			break;

		case 1:
			m_Window.GetRenderer().EnabledDeferred();
			break;
		}
	}
}