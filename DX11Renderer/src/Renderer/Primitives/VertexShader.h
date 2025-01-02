#pragma once
#include "Renderer/RendererContext.h"
#include "Renderer/Primitives/InputLayout.h"
#include <memory>

namespace Yassin
{
	class VertexShader
	{
	public:
		VertexShader(wchar_t* shaderFile);
		
		inline void Bind() 
		{
			m_InputLayout->Bind();
			RendererContext::GetDeviceContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0);
		}
		inline ID3DBlob* GetBlob() const { return m_Blob.Get(); }
		inline InputLayout* GetInputLayout() { return m_InputLayout.get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_Blob;
		std::unique_ptr<InputLayout> m_InputLayout;
	};
}
