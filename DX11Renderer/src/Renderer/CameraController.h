#pragma once
#include "Renderer/Camera.h"

namespace Yassin
{
	class CameraController
	{
	public:
		CameraController(float FOV, float aspectRatio);
		
		void OnUpdate(float dt);

		Camera& GetCamera() { return m_Camera; }

	private:
		float m_FOV;
		float m_AspectRatio;
		float m_CameraMoveSpeed = 1.f;
		float m_CameraRotationSpeed = 45.f;
		DirectX::XMFLOAT3 m_Position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		float m_Yaw = 0.f;
		float m_Pitch = 0.f;
		float m_Roll = 0.f;
		float m_MouseSensitivity = 0.1f;
		long m_MouseX = 0;
		long m_MouseXPrev = 0;
		long m_MouseY = 0;
		long m_MouseYPrev = 0;

		Camera m_Camera;
	};
}
