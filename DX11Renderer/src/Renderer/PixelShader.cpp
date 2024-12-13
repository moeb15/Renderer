#include "Renderer/PixelShader.h"


namespace Yassin
{
	PixelShader::PixelShader(wchar_t* shaderFile)
	{
		HRESULT hr;
		hr = D3DReadFileToBlob(shaderFile, &m_Blob);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to read file to blob", shaderFile, MB_OK);
			return;
		}

		hr = RendererContext::GetDevice()->CreatePixelShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_PixelShader);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create pixel shader", shaderFile, MB_OK);
			return;
		}
	}
}