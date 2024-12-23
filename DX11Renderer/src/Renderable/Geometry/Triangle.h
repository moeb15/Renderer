#pragma once
#include "Renderer/Renderable.h"
#include "Renderer/MaterialInstance.h"
#include "Renderable/DataTypes.h"
#include <string>

namespace Yassin
{
	class Triangle : public Renderable
	{
	public:
		Triangle(std::string material, DirectX::XMMATRIX world);
		virtual void Render(DirectX::XMMATRIX& viewProj) const override;
		virtual void UpdateLighting(const LightPositionBuffer& lPos, const LightPropertiesBuffer& lProps) const override;

	private:
		Topology m_Topology;
	};
}