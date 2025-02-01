#pragma once
#include "Renderable/Renderable.h"
#include "Renderer/Resources/MaterialInstance.h"
#include "Renderable/DataTypes.h"
#include <string>

namespace Yassin
{
	class Plane : public Renderable
	{
	public:
		Plane(std::string material, DirectX::XMMATRIX world, int width, int height, float dimX, float dimY);
		void GeneratePlane();

	private:
		int m_Width;
		int m_Height;
		float m_X;
		float m_Y;
	};
}
