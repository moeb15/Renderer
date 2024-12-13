#pragma once
#include "Renderer/RendererContext.h"

namespace Yassin
{
	class VertexShader
	{
	public:
		VertexShader(wchar_t* shaderFile);
		
		inline void Bind() 
		{
			RendererContext::GetDeviceContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0);
		}
		inline ID3DBlob* GetBlob() const { return m_Blob.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_Blob;

	};
}
