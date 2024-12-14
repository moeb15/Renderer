#include "Application.h"
#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"
#include "Renderable/Triangle.h"

namespace Yassin
{
	Application::Application(const ApplicationSpecification& appSpec) :
		m_Window(appSpec.width, appSpec.height, appSpec.name)
	{
		m_Window.Init();

		DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

		DirectX::XMMATRIX projection = DirectX::XMMatrixOrthographicLH(16.f,9.f, -1.f, 1.f);

		DirectX::XMFLOAT3 up, pos, lookAt;
		DirectX::XMVECTOR upVec, posVec, lookAtVec;
		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;


		upVec = DirectX::XMLoadFloat3(&up);
		pos.x = 0.0f;
		pos.y = 0.0f;
		pos.z = 0.0f;

		posVec = DirectX::XMLoadFloat3(&pos);

		lookAt.x = 0.0f;
		lookAt.y = 0.0f;
		lookAt.z = 1.0f;

		lookAtVec = DirectX::XMLoadFloat3(&lookAt);

		DirectX::XMMATRIX rotMat;

		rotMat = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.f, 0.f);

		lookAtVec = DirectX::XMVector3TransformCoord(lookAtVec, rotMat);
		upVec = DirectX::XMVector3TransformCoord(upVec, rotMat);
		lookAtVec = DirectX::XMVectorAdd(posVec, lookAtVec);

		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(posVec, lookAtVec, upVec);

		testTriangle = std::make_shared<Triangle>("Test Shader", world, view, projection);
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
		
		m_Window.GetRenderer().BeginScene(0.2f, 0.2f, 0.2f, 1.0f);

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		bool static showDemo = true;

		ImGui::ShowDemoWindow(&showDemo);

		m_Window.GetRenderer().Submit(testTriangle.get());

		m_Window.GetRenderer().Render();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		m_Window.GetRenderer().EndScene();
	}
}