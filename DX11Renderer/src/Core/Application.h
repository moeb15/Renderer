#pragma once
#include "Core/Window.h"
#include "Core/Timer.h"
#include "Renderer/Camera/CameraController.h"
#include "Renderable/RenderablePrimitives.h"
#include "Scenes/SponzaScene.h"
#include "Scenes/TestScene.h"

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
		void SettingsGui();

	private:
		Window m_Window;
		Timer m_Timer;
		CameraController m_CameraController;
		std::unique_ptr<PointLight> light;
		std::unique_ptr<DirectionalLight> sun;
		std::unique_ptr<SponzaScene> m_SponzaScene;
		std::unique_ptr<TestScene> m_TestScene;
		Scene* m_ActiveScene;
		char m_GPUName[128];
		size_t m_GPUMem;
	};
}
