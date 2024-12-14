#pragma once
#include "Renderer/RendererContext.h"
#include <vector>

namespace Yassin
{
	class IndexBuffer
	{
	public:
		inline IndexBuffer(std::vector<unsigned long>& data)
		{
			D3D11_BUFFER_DESC iDesc = {};
			iDesc.Usage = D3D11_USAGE_DEFAULT;
			iDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			iDesc.ByteWidth = sizeof(unsigned long) * std::size(data);

			D3D11_SUBRESOURCE_DATA iData = {};
			iData.pSysMem = data.data();

			RendererContext::GetDevice()->CreateBuffer(&iDesc, &iData, &m_IndexBuffer);
			m_Count = std::size(data);
		}
		inline void Bind() const
		{
			RendererContext::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		}

		inline unsigned int GetIndexCount() const { return m_Count; }

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
		unsigned int m_Count;
	};

}