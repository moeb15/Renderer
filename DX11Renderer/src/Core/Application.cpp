#include "Application.h"
#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"
#include "Renderable/Triangle.h"

namespace Yassin
{
	Application::Application(const ApplicationSpecification& appSpec) :
		m_Window(appSpec.width, appSpec.height, appSpec.name),
		m_CameraController(45.f, appSpec.width / appSpec.height)
	{
		m_Window.Init();

		DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		testTriangle = std::make_shared<Triangle>("Test Shader", world);
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

		ImGui::ShowDemoWindow(&showDemo);

		m_Window.GetRenderer().Submit(testTriangle.get());

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