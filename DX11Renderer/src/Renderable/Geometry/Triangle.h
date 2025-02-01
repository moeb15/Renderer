#pragma once
#include "Renderable/Renderable.h"
#include "Renderer/Resources/MaterialInstance.h"
#include "Renderable/DataTypes.h"
#include <string>

namespace Yassin
{
	class Triangle : public Renderable
	{
	public:
		Triangle(std::string material, DirectX::XMMATRIX world);
	};
}