#pragma once
#include "Renderer/RendererContext.h"
#include <memory>

namespace Yassin
{
	class ComputeShader
	{
	public:
		ComputeShader(wchar_t* shaderFile);

		inline void Bind()
		{
			RendererContext::GetDeviceContext()->CSSetShader(m_ComputeShader.Get(), nullptr, 0);
		}

		inline void SetSRV(unsigned int slot, ID3D11ShaderResourceView* srv)
		{
			RendererContext::GetDeviceContext()->CSSetShaderResources(slot, 1, &srv);
		}

		inline void SetUAV(unsigned int slot, ID3D11UnorderedAccessView* uav)
		{
			RendererContext::GetDeviceContext()->CSSetUnorderedAccessViews(slot, 1, &uav, nullptr);
		}

		inline ID3DBlob* GetBlob() const { return m_Blob.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_ComputeShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_Blob;
	};
}
