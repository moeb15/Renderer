#pragma once
#include "Renderer/Renderable.h"
#include "Renderable/DataTypes.h"
#include "Renderer/Resources/MaterialInstance.h"

namespace Yassin
{
	class Mesh : public Renderable
	{
	public:
		Mesh(std::string material, DirectX::XMMATRIX modelWorld, DirectX::XMMATRIX meshWorld, std::vector<Vertex>& vertices,
			std::vector<unsigned long>& indices, float boundsX, float boundsY, float boundsZ, const char* meshName,
			std::vector<InstancePosition>* instancePositions = nullptr);

	private:
		std::string m_Name;
	};
}
