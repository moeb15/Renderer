#pragma once
#include "Core/Window.h"
#include "Core/Timer.h"
#include "Renderer/CameraController.h"
#include "Renderable/RenderablePrimitives.h"

namespace Yassin
{
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
		std::unique_ptr<Box> box;
		std::unique_ptr<Plane> plane;
		std::unique_ptr<PointLight> light;
		char m_GPUName[128];
		int m_GPUMem;
	};
}
