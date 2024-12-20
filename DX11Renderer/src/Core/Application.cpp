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

		DirectX::XMMATRIX world;

		world = DirectX::XMMatrixTranslation(0.f, 0.f, 5.f);
		box = std::make_unique<Box>("Depth Material", world);

		//world = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(90.f));
		world = DirectX::XMMatrixTranslation(37.5f, -0.5f, -30.f);
		plane = std::make_unique<Plane>("Depth Material", world, 100, 100, 25.f, 25.f);

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

		//ImGui::ShowDemoWindow(&showDemo);
		//testTriangle->Rotate(0, 0, dt);
		if(ImGui::Begin("Statistics"))
		{
			ImGui::Text("FPS : %f", ImGui::GetIO().Framerate);
			ImGui::Text("Renderer : %s", m_GPUName);
			ImGui::Text("VRAM : %i", m_GPUMem);
		}
		ImGui::End();

		m_Window.GetRenderer().Submit(box.get());
		m_Window.GetRenderer().Submit(plane.get());

		m_Window.GetRenderer().Render(m_CameraController.GetCamera());

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