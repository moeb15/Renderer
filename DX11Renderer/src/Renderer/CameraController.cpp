#include "Renderer/CameraController.h"
#include "Core/Input.h"
#include <Windows.h>

namespace Yassin
{
	CameraController::CameraController(float FOV, float aspectRatio):
		m_FOV(FOV),
		m_AspectRatio(aspectRatio),
		m_Camera(45.f, aspectRatio)
	{
	}

	void CameraController::OnUpdate(float dt)
	{
		if(Input::IsKeyDown('A'))
		{
			m_Position.x += m_CameraMoveSpeed * dt;
			m_Camera.SetPosition(m_Position.x, m_Position.y, m_Position.z);
		}
		if(Input::IsKeyDown('D'))
		{
			m_Position.x -= m_CameraMoveSpeed * dt;
			m_Camera.SetPosition(m_Position.x, m_Position.y, m_Position.z);
		}
		if(Input::IsKeyDown('W'))
		{
			m_Position.z += m_CameraMoveSpeed * dt;
			m_Camera.SetPosition(m_Position.x, m_Position.y, m_Position.z);
		}
		if(Input::IsKeyDown('S'))
		{
			m_Position.z -= m_CameraMoveSpeed * dt;
			m_Camera.SetPosition(m_Position.x, m_Position.y, m_Position.z);
		}

		if (Input::ShouldProcessRawMouseMove())
		{
			std::pair<long, long> mousePos = Input::GetPosRaw();

			m_MouseX += mousePos.first;
			if(m_MouseX != m_MouseXPrev)
			{
				m_Yaw = m_MouseX * m_MouseSensitivity;
				m_MouseXPrev = m_MouseX;
			}

			m_MouseY += mousePos.second;
			if (m_MouseY != m_MouseYPrev)
			{
				m_Pitch = m_MouseY * m_MouseSensitivity;
				m_MouseYPrev = m_MouseY;
			}

			if (m_Pitch > 89.f) m_Pitch = 89.f;
			if (m_Pitch < -89.f) m_Pitch = -89.f;

			m_Camera.SetRotation(m_Yaw, m_Pitch, m_Roll);
		}
	}
}