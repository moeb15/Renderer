#include "Renderer/InputLayout.h"

namespace Yassin
{
	InputLayout::InputLayout(ID3DBlob* pBlob, const D3D11_INPUT_ELEMENT_DESC* ied, unsigned int size)
	{
		HRESULT hr;	
		hr = RendererContext::GetDevice()->CreateInputLayout(ied, size, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_InputLayout);
		if(FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create input layout", L"Error", MB_OK);
			m_IED = nullptr;
			m_Size = 0;
			return;
		}
		m_IED = ied;
		m_Size = size;
	}
}