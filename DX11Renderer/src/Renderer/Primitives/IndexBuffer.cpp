#include "Renderer/Primitives/IndexBuffer.h"

Yassin::IndexBuffer::IndexBuffer(std::vector<unsigned long>& data)
{
	D3D11_BUFFER_DESC iDesc = {};
	iDesc.Usage = D3D11_USAGE_DEFAULT;
	iDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	iDesc.ByteWidth = (unsigned int)(sizeof(unsigned long) * std::size(data));

	D3D11_SUBRESOURCE_DATA iData = {};
	iData.pSysMem = data.data();

	RendererContext::GetDevice()->CreateBuffer(&iDesc, &iData, &m_IndexBuffer);
	m_Count = (unsigned int)std::size(data);
}
