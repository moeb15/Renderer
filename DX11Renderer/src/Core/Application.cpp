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

		DirectX::XMMATRIX world;
		for(int i = 0; i < 10000; i++)
		{
			float x, y, z;
			x = (i % 2) == 0 ? i : -i;
			y = (i % 2) == 0 ? -i : i;
			z = 10 * sin(i);
			world = DirectX::XMMatrixTranslation(x, y, z);;
			triangles.push_back(std::make_unique<Triangle>("Texture Material", world));
		}
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
		for(int i = 0; i < triangles.size(); i++)
			m_Window.GetRenderer().Submit(triangles[i].get());

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