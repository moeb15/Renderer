#pragma once
#include <DirectXMath.h>

namespace Yassin
{
	struct Vertex
	{
		Vertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 tex, DirectX::XMFLOAT3 norm) : 
			position(pos), 
			uv(tex),
			normal(norm) {}

		Vertex() :
			position(0.0f, 0.0f, 0.0f),
			uv(0.0f, 0.0f),
			normal(0.0f, 0.0f, 0.0f) {}

		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
	};
}