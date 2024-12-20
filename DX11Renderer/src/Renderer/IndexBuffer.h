#pragma once
#include "Renderer/RendererContext.h"
#include <vector>

namespace Yassin
{
	class IndexBuffer
	{
	public:
		IndexBuffer(std::vector<unsigned long>& data);

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