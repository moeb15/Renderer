#pragma once
#include <DirectXCollision.h>

namespace Yassin
{
	class PointLight
	{
	public:
		PointLight(float FOV, float aspectRatio, float sNear, float sFar, float radius = 1.0f) :
			m_AmbientColor(0.1f, 0.1f, 0.1f, 1.0f),
			m_DiffuseColor(1.0f, 1.0f, 1.0f, 1.0f),
			m_SpecularColor(1.f, 1.f, 1.f, 1.0f),
			m_SpecularPower(32.f),
			m_Position(0.f, 10.f, 0.f),
			m_LookAt(0.f, 0.f, 0.f)
		{
			m_Enabled = false;
			m_Radius = radius;

			DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(
				DirectX::XMConvertToRadians(FOV),
				aspectRatio,
				sNear,
				sFar);
			DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, proj);
			UpdateView();

			m_BoundingSphere = DirectX::BoundingSphere(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), radius);
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

		inline void SetSpecularPower(float x) { m_SpecularPower = x; }

		inline void SetPosition(float x, float y, float z)
		{
			m_Position = DirectX::XMFLOAT3(x, y, z);

			m_BoundingSphere.Transform(m_BoundingSphere, DirectX::XMMatrixTranslation(x, y, z));
			UpdateView();
		}

		inline void SetLookAt(float x, float y, float z)
		{
			m_LookAt = DirectX::XMFLOAT3(x, y, z);
			UpdateView();
		}

		inline DirectX::XMFLOAT3 GetPosition() const { return m_Position; }
		inline DirectX::XMFLOAT3 GetLookAt() const { return m_LookAt; }
		inline DirectX::XMFLOAT4 GetDiffuseColor() const { return m_DiffuseColor; }
		inline DirectX::XMFLOAT4 GetAmbientColor() const { return m_AmbientColor; }
		inline DirectX::XMFLOAT4 GetSpecularColor() const { return m_SpecularColor; }
		inline float GetSpecularPower() const { return m_SpecularPower; }
		inline float GetRadius() const { return m_Radius; }
		inline bool IsEnabled() const { return m_Enabled; }
		inline void EnableLight() { m_Enabled = true; }
		inline void DisableLight() { m_Enabled = false; }

		inline void GetView(DirectX::XMMATRIX& matrix) const { matrix = DirectX::XMLoadFloat4x4(&m_ViewMatrix); }
		inline void GetProjection(DirectX::XMMATRIX& matrix) const { matrix = DirectX::XMLoadFloat4x4(&m_ProjectionMatrix); }
		inline DirectX::BoundingSphere& GetBoundingSphere() { return m_BoundingSphere; }

	private:
		void UpdateView();
	private:
		DirectX::XMFLOAT4 m_AmbientColor;
		DirectX::XMFLOAT4 m_DiffuseColor;
		DirectX::XMFLOAT4 m_SpecularColor;
		float m_SpecularPower;
		bool m_Enabled;
		float m_Radius;

		DirectX::XMFLOAT3 m_Position;
		DirectX::XMFLOAT3 m_LookAt;
		DirectX::XMFLOAT4X4 m_ViewMatrix;
		DirectX::XMFLOAT4X4 m_ProjectionMatrix;

		DirectX::BoundingSphere m_BoundingSphere;
	};
}
