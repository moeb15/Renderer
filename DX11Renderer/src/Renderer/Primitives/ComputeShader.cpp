#include "Renderer/Primitives/ComputeShader.h"


namespace Yassin
{
	ComputeShader::ComputeShader(wchar_t* shaderFile)
	{
		HRESULT hr;
		hr = D3DReadFileToBlob(shaderFile, &m_Blob);
		if(FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to read file to blob", shaderFile, MB_OK);
			return;
		}

		hr = RendererContext::GetDevice()->CreateComputeShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_ComputeShader);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create compute shader", shaderFile, MB_OK);
			return;
		}
	}
}
