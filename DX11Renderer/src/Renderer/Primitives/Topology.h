#pragma once
#include "Renderer/RendererContext.h"

namespace Yassin
{
	class Topology
	{
	public:
		Topology(D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) : m_Topology(topology) {}
		
		inline void Bind() const
		{
			RendererContext::GetDeviceContext()->IASetPrimitiveTopology(m_Topology);
		}

		inline D3D11_PRIMITIVE_TOPOLOGY GetTopology() const { return m_Topology; }
		inline void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { m_Topology = topology; }

	private:
		D3D11_PRIMITIVE_TOPOLOGY m_Topology;
	};
}
