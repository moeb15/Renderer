#pragma once
#include "Renderer/Renderable.h"
#include "Renderer/Resources/MaterialInstance.h"
#include "Renderable/DataTypes.h"
#include <string>

namespace Yassin
{
	class Model : public Renderable
	{
	public:
		Model(std::string material, const std::string& modelFile, DirectX::XMMATRIX world, std::vector<InstancePosition>* instancePositions = nullptr);
	};
}
