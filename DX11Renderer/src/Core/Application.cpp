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

		light = std::make_unique<PointLight>(90.f, 0.1f, 1000.f);
		light->SetPosition(37.f, 10.f, 3.f);

		DirectX::XMMATRIX world;

		world = DirectX::XMMatrixTranslation(0.f, 0.f, 5.f);
		box = std::make_unique<Box>("Shadow Map Material", world);

		world = DirectX::XMMatrixTranslation(37.5f, -0.5f, -30.f);
		plane = std::make_unique<Plane>("Shadow Map Material", world, 100, 100, 25.f, 25.f);

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


		m_Window.GetRenderer().BeginScene(0.2f, 0.2f, 0.2f, 1.0f);

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		bool static showDemo = true;

		DirectX::XMMATRIX lView;
		DirectX::XMMATRIX lProj;

		DirectX::XMFLOAT3 lightPosition = light->GetPosition();
		light->GetView(lView);
		light->GetProjection(lProj);

		if(ImGui::Begin("Light Position"))
		{
			ImGui::SliderFloat("X", &lightPosition.x, -50.f, 50.f, "%.1f");
			ImGui::SliderFloat("Y", &lightPosition.y, -50.f, 50.f, "%.1f");
			ImGui::SliderFloat("Z", &lightPosition.z, -50.f, 50.f, "%.1f");
			
			ImGui::Text("Statistics");
			ImGui::Text("FPS : %f", ImGui::GetIO().Framerate);
			ImGui::Text("Renderer : %s", m_GPUName);
			ImGui::Text("VRAM : %i", m_GPUMem);
		}
		ImGui::End();

		light->SetPosition(lightPosition.x, lightPosition.y, lightPosition.z);

		box->UpdateLighting(
			{ DirectX::XMMatrixMultiply(lView, lProj), light->GetPosition() },
			{ light->GetAmbientColor(), light->GetDiffuseColor(), light->GetSpecularColor(),
			light->GetSpecularPower() });

		box->Rotate(dt * 10.f, 0.0f, 0.0f);

		plane->UpdateLighting(
			{ DirectX::XMMatrixMultiply(lView, lProj), light->GetPosition() },
			{ light->GetAmbientColor(), light->GetDiffuseColor(), light->GetSpecularColor(),
			light->GetSpecularPower() });

		m_Window.GetRenderer().Submit(box.get());
		m_Window.GetRenderer().Submit(plane.get());

		m_Window.GetRenderer().Render(m_CameraController.GetCamera(), DirectX::XMMatrixMultiply(lView, lProj));

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		m_Window.GetRenderer().EndScene();
	}
}