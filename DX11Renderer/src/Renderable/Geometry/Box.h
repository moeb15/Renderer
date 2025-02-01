#pragma once
#include "Renderable/Renderable.h"
#include "Renderer/Resources/MaterialInstance.h"
#include "Renderable/DataTypes.h"
#include <string>

namespace Yassin
{
	class Box : public Renderable
	{
	public:
		Box(std::string material, DirectX::XMMATRIX world, std::vector<InstancePosition>* instancePositions = nullptr);

	};
}

