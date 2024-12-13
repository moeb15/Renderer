#pragma once
#include "Renderer/RendererContext.h"
#include <vector>

namespace Yassin
{
	template<typename C>
	class ConstantBuffer
	{
	public:
		ConstantBuffer()
		{
			D3D11_BUFFER_DESC cDesc = {};
			cDesc.Usage =  D3D11_USAGE_DYNAMIC;
			cDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cDesc.ByteWidth = sizeof(C);
			cDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			RendererContext::GetDevice()->CreateBuffer(&cDesc, nullptr, &m_ConstantBuffer);
		}

		void UpdateBuffer(C& data)
		{
			HRESULT hr;
			D3D11_MAPPED_SUBRESOURCE mRes = {};
			C* cData;

			hr = RendererContext::GetDeviceContext()->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mRes);
			if(FAILED(hr))
			{
				MessageBox(RendererContext::GetWindowHandle(), L"Failed to map to constant buffer", L"Error", MB_OK);
				return;
			}

			cData = (C*)mRes.pData;
			cData = &data;

			RendererContext::GetDeviceContext()->Unmap(m_ConstantBuffer.Get(), 0);
		}
	
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;
	};
}