#pragma once
#include <DirectXMath.h>

namespace Yassin
{
	class Camera
	{
	public:
		Camera(float FOV, float aspectRatio);

		inline void SetPosition(float x, float y, float z)
		{
			m_Position.x = x;
			m_Position.y = y;
			m_Position.z = z;

			UpdateView();
		}

		inline void SetRotation(float yaw, float pitch, float roll)
		{
			m_Yaw = yaw;
			m_Pitch = pitch;
			m_Roll = roll;

			UpdateView();
		}

		inline DirectX::XMFLOAT3 GetPosition() const { return m_Position; };
		inline DirectX::XMFLOAT3 GetRotation() { return DirectX::XMFLOAT3(m_Pitch, m_Yaw, m_Roll); }

		inline void GetViewMatrix(DirectX::XMMATRIX& matrix) { matrix = DirectX::XMLoadFloat4x4(&m_ViewMatrix); }
		inline void GetDefaultView(DirectX::XMMATRIX& matrix) { matrix = DirectX::XMLoadFloat4x4(&m_DefaultView); }
		inline void GetProjectionMatrix(DirectX::XMMATRIX& matrix) { matrix = DirectX::XMLoadFloat4x4(&m_ProjectionMatrix); }

		inline const DirectX::XMFLOAT3& GetRightVector() const { return m_Right; }
		inline const DirectX::XMFLOAT3& GetForwardVector() const { return m_Forward; }
		inline const DirectX::XMFLOAT3& GetUpVector() const { return m_Up; }

	private:
		void UpdateView();

	private:
		DirectX::XMFLOAT3 m_Position;
		float m_Yaw;
		float m_Pitch;
		float m_Roll;
		DirectX::XMFLOAT3 m_Forward;
		DirectX::XMFLOAT3 m_Right;
		DirectX::XMFLOAT3 m_Up;

		DirectX::XMFLOAT4X4 m_ViewMatrix;
		DirectX::XMFLOAT4X4 m_DefaultView;
		DirectX::XMFLOAT4X4 m_ProjectionMatrix;
	};
}
