#pragma once
#include "Renderer/Renderable.h"
#include "Renderable/DataTypes.h"
#include "Renderer/Resources/MaterialInstance.h"

namespace Yassin
{
	class Mesh : public Renderable
	{
	public:
		Mesh(std::string material, DirectX::XMMATRIX world, std::vector<Vertex>& vertices,
			std::vector<unsigned long>& indices,std::vector<InstancePosition>* instancePositions = nullptr);

	};
}
