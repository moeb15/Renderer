#pragma once
#include "Renderer/Renderable.h"
#include "Renderer/Resources/MaterialInstance.h"
#include "Renderable/DataTypes.h"
#include <string>

namespace Yassin
{
	class Box : public Renderable
	{
	public:
		Box(std::string material, DirectX::XMMATRIX world, std::vector<InstancePosition>* instancePositions = nullptr);
		virtual void Render(DirectX::XMMATRIX& viewProj, bool bIgnoreMaterial) const override;
		virtual void UpdateLighting(const LightPositionBuffer& lPos, const LightPropertiesBuffer& lProps) const override;

	private:
		Topology m_Topology;
	};
}

