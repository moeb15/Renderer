#pragma once
#include "Renderer/RendererContext.h"
#include <vector>

namespace Yassin
{
	class InputLayout
	{
	public:
		InputLayout(ID3DBlob* pBlob, const D3D11_INPUT_ELEMENT_DESC* ied, unsigned int size);
		
		inline void Bind()
		{
			RendererContext::GetDeviceContext()->IASetInputLayout(m_InputLayout.Get());
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	};
}
