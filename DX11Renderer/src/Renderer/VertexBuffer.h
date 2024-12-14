#pragma once
#include "Renderer/RendererContext.h"
#include <vector>

namespace Yassin
{
	class VertexBuffer
	{
	public:
		template<typename V>
		VertexBuffer(std::vector<V>& data);
		inline void Bind(unsigned int slot) const
		{
			RendererContext::GetDeviceContext()->IASetVertexBuffers(slot, 1, m_VertexBuffer.GetAddressOf(),
				&m_Strides, &m_Offset);
		}
	
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;

		unsigned int m_Strides;
		unsigned int m_Offset;
	};

	template<typename V>
	inline VertexBuffer::VertexBuffer(std::vector<V>& data)
	{
		D3D11_BUFFER_DESC vDesc = {};
		vDesc.Usage = D3D11_USAGE_DEFAULT;
		vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vDesc.ByteWidth = sizeof(V) * std::size(data);

		D3D11_SUBRESOURCE_DATA vData = {};
		vData.pSysMem = data.data();

		RendererContext::GetDevice()->CreateBuffer(&vDesc, &vData, &m_VertexBuffer);
		m_Strides = sizeof(V);
		m_Offset = 0;
	}
}
