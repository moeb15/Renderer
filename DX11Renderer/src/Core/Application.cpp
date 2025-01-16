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

		sun = std::make_unique<DirectionalLight>(1000.f, 0.1f, 1000.f);
		sun->SetDirection(0.0f, 0.0f, -1.f);

		light = std::make_unique<PointLight>(90.f, 1.f, 1.f, 100.f);
		light->SetAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
		light->SetPosition(0.0f, 10.f, -6.f);
		light->SetLookAt(0.0f, 0.0f, 0.0f);

		DirectX::XMMATRIX world;
		DirectX::XMMATRIX rot;
		DirectX::XMMATRIX translate;

		std::vector<InstancePosition> boxPositions =
		{
			{{0.0f, 0.0f, 0.0f}},
			{{-10.f, 0.0f, 0.0f}}
		};

		world = DirectX::XMMatrixTranslation(2.f, 0.f, 0.f);
		box = std::make_unique<Box>("Shadow Map Material", world, &boxPositions);
		
		world = DirectX::XMMatrixTranslation(1.5f, 0.f, 2.f);
		transparentBox = std::make_unique<Box>("Shadow Map Material", world);
		transparentBox->SetObjectVisiblity(ObjectVisibility::Transparent);
		transparentBox->UpdateTransparency(0.5f);

		world = DirectX::XMMatrixIdentity();
		rot = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(90.f));
		world = DirectX::XMMatrixTranslation(37.5f, -0.5f, -30.f);
		//world = DirectX::XMMatrixMultiply(rot, world);
		plane = std::make_unique<Plane>("Shadow Map Material", world, 100, 100, 25.f, 25.f);


		world = DirectX::XMMatrixIdentity();
		world = DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f);
		translate = DirectX::XMMatrixTranslation(0.0f, -0.5f, 0.f);
		world = DirectX::XMMatrixMultiply(world, translate);
		testModel = std::make_unique<Model>("Shadow Map Material", "src/Assets/Models/Nanosuit/nanosuit.obj", world, &boxPositions);

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

		DirectX::XMMATRIX lView;
		DirectX::XMMATRIX lProj;
		DirectX::XMMATRIX lViewProj;

		SettingsGui();

		light->GetView(lView);
		light->GetProjection(lProj);

		lViewProj = DirectX::XMMatrixMultiply(lView, lProj);

		box->UpdateLighting(
			{ lViewProj, light->GetPosition() },
			{ light->GetAmbientColor(), light->GetDiffuseColor(), light->GetSpecularColor(),
			light->GetSpecularPower() });

		//box->Rotate(dt * 10.f, 0.0f, 0.0f);
		
		//testModel->Rotate(dt * 10.f, 0.0f, 0.0f);

		plane->UpdateLighting(
			{ lViewProj, light->GetPosition() },
			{ light->GetAmbientColor(), light->GetDiffuseColor(), light->GetSpecularColor(),
			light->GetSpecularPower() });

		transparentBox->UpdateLighting(
			{ lViewProj, light->GetPosition() },
			{ light->GetAmbientColor(), light->GetDiffuseColor(), light->GetSpecularColor(),
			light->GetSpecularPower() });

		testModel->UpdateLighting(
			{ lViewProj, light->GetPosition() },
			{ light->GetAmbientColor(), light->GetDiffuseColor(), light->GetSpecularColor(),
			light->GetSpecularPower() });

		testModel->UpdateCameraPosition({ m_CameraController.GetCamera().GetPosition() });
		testModel->UpdateLightDirection({ sun->GetDirection() });

		m_Window.GetRenderer().Submit(testModel.get());

		//m_Window.GetRenderer().Submit(transparentBox.get());

		//m_Window.GetRenderer().Submit(box.get());
		m_Window.GetRenderer().Submit(plane.get());

		m_Window.GetRenderer().Render(m_CameraController.GetCamera(), lViewProj);

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
		DirectX::XMFLOAT3 lightPosition = light->GetPosition();
		DirectX::XMFLOAT3 lightLookAt = light->GetLookAt();
		DirectX::XMFLOAT3 lightDir = sun->GetDirection();

		if (ImGui::Begin("Settings"))
		{
			ImGui::Text("Post-Processing Settings");
			ImGui::Dummy(ImVec2(0.0f, 1.0f));
			ImGui::Checkbox("Toggle Post-Processing", &m_Window.GetRenderer().PostProcessingEnabled());

			if (m_Window.GetRenderer().PostProcessingEnabled())
			{
				ImGui::Text("Blur");
				ImGui::Dummy(ImVec2(0.0f, 1.0f));
				ImGui::Checkbox("Gaussian Blur", &m_Window.GetRenderer().GaussianBlurEnabled());
				ImGui::Checkbox("Box Blur", &m_Window.GetRenderer().BoxBlurEnabled());
			}

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::Text("Directional Light Settings");
			ImGui::Dummy(ImVec2(0.0f, 1.0f));
			ImGui::SliderFloat("X-Dir", &lightDir.x, -50.f, 50.f, "%.1f");
			ImGui::SliderFloat("Y-Dir", &lightDir.y, -50.f, 50.f, "%.1f");
			ImGui::SliderFloat("Z-Dir", &lightDir.z, -50.f, 50.f, "%.1f");

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

		light->SetPosition(lightPosition.x, lightPosition.y, lightPosition.z);
		light->SetLookAt(lightLookAt.x, lightLookAt.y, lightLookAt.z);
		sun->SetDirection(lightDir.x, lightDir.y, lightDir.z);
	}
}