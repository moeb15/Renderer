#pragma once
#include <DirectXMath.h>

namespace Yassin
{
	class DirectionalLight
	{
	public:
		DirectionalLight(float size, float sNear, float sFar) :
			m_AmbientColor(0.1f, 0.1f, 0.1f, 1.0f),
			m_DiffuseColor(1.0f, 1.0f, 1.0f, 1.0f),
			m_SpecularColor(1.f, 1.f, 1.f, 1.0f),
			m_SpecularPower(32.f),
			m_Position(0.f, 10.f, 0.f),
			m_LookAt(0.f, 0.f, 0.f),
			m_Direction(0.0f, -1.0f, 0.0f)
		{
			DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicLH(
				size,
				size,
				sNear,
				sFar);

			DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, proj);
			UpdateView();
		}
		inline void SetDiffuseColor(float r, float g, float b, float a)
		{
			m_DiffuseColor = DirectX::XMFLOAT4(r, g, b, a);
		}

		inline void SetAmbientColor(float r, float g, float b, float a)
		{
			m_AmbientColor = DirectX::XMFLOAT4(r, g, b, a);
		}

		inline void SetSpecularColor(float r, float g, float b, float a)
		{
			m_SpecularColor = DirectX::XMFLOAT4(r, g, b, a);
		}

		inline void SetSepcularPower(float x) { m_SpecularPower = x; }

		inline void SetPosition(float x, float y, float z)
		{
			m_Position = DirectX::XMFLOAT3(x, y, z);
			UpdateView();
		}

		inline void SetLookAt(float x, float y, float z)
		{
			m_LookAt = DirectX::XMFLOAT3(x, y, z);
			UpdateView();
		}

		inline void SetDirection(float x, float y, float z)
		{
			m_Direction = DirectX::XMFLOAT3(x, y, z);
		}

		inline DirectX::XMFLOAT3 GetPosition() const { return m_Position; }
		inline DirectX::XMFLOAT3 GetDirection() const { return m_Direction; }
		inline DirectX::XMFLOAT3 GetLookAt() const { return m_LookAt; }
		inline DirectX::XMFLOAT4 GetDiffuseColor() const { return m_DiffuseColor; }
		inline DirectX::XMFLOAT4 GetAmbientColor() const { return m_AmbientColor; }
		inline DirectX::XMFLOAT4 GetSpecularColor() const { return m_SpecularColor; }
		inline float GetSpecularPower() const { return m_SpecularPower; }

		inline void GetView(DirectX::XMMATRIX& matrix) const { matrix = DirectX::XMLoadFloat4x4(&m_ViewMatrix); }
		inline void GetProjection(DirectX::XMMATRIX& matrix) const { matrix = DirectX::XMLoadFloat4x4(&m_ProjectionMatrix); }

	private:
		void UpdateView();

	private:
		DirectX::XMFLOAT4 m_AmbientColor;
		DirectX::XMFLOAT4 m_DiffuseColor;
		DirectX::XMFLOAT4 m_SpecularColor;
		float m_SpecularPower;

		DirectX::XMFLOAT3 m_Position;
		DirectX::XMFLOAT3 m_Direction;
		DirectX::XMFLOAT3 m_LookAt;
		DirectX::XMFLOAT4X4 m_ViewMatrix;
		DirectX::XMFLOAT4X4 m_ProjectionMatrix;
	};
}
