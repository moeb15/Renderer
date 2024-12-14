#pragma once
#include "Core/Window.h"
#include "Core/Timer.h"
#include "Renderer/CameraController.h"

namespace Yassin
{
	class Triangle;
	struct ApplicationSpecification
	{
		wchar_t* name = L"DX11Renderer";
		int width = 1920;
		int height = 1080;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& appSpec);
		~Application();
		int Run();
		
	private:
		void Frame();

	private:
		Window m_Window;
		Timer m_Timer;
		CameraController m_CameraController;
		std::shared_ptr<Triangle> testTriangle;
	};
}
