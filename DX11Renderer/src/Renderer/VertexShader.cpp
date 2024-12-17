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
		// TODO: Generalize input layout
		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		m_InputLayout = std::make_unique<InputLayout>(m_Blob.Get(), ied, std::size(ied));
	}
}