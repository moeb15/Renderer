#pragma once
#include "Renderer/Renderable.h"
#include "Renderer/MaterialInstance.h"
#include "Renderable/DataTypes.h"
#include <string>

namespace Yassin
{
	class Box : public Renderable
	{
	public:
		Box(std::string material, DirectX::XMMATRIX world);
		virtual void Render(DirectX::XMMATRIX& viewProj) const override;
		virtual void UpdateLighting(const LightPositionBuffer& lPos, const LightPropertiesBuffer& lProps) const override;
		virtual void UpdateShadowMap(ID3D11ShaderResourceView* srv) const override;
		virtual void UnbindSRV() const override;

	private:
		Topology m_Topology;
	};
}

