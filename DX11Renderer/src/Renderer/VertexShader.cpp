#include "Renderer/VertexShader.h"

namespace Yassin
{
	VertexShader::VertexShader(wchar_t* shaderFile)
	{
		HRESULT hr;
		hr = D3DReadFileToBlob(shaderFile, &m_Blob);
		if(FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to read file to blob", shaderFile, MB_OK);
			return;
		}

		hr = RendererContext::GetDevice()->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_VertexShader);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create vertex shader", shaderFile, MB_OK);
			return;
		}
	}
}