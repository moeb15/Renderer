#pragma once
#include "Renderer/RendererContext.h"
#include <vector>

namespace Yassin
{
	struct InstancePosition
	{
		DirectX::XMFLOAT3 position;
	};

	class VertexBuffer
	{
	public:
		template<typename V>
		VertexBuffer(std::vector<V>& data, std::vector<InstancePosition>* instanceData = nullptr);
		inline void Bind(unsigned int slot) const
		{
			RendererContext::GetDeviceContext()->IASetVertexBuffers(slot, 2, m_BufferPointers,
				m_Strides, m_Offset);
		}
	
		inline size_t GetInstanceCount() const { return m_InstanceCount; }
		inline unsigned int GetVertexCount() const { return m_VertexCount; }

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_InstanceBuffer;

		unsigned int m_Strides[2];
		unsigned int m_Offset[2];
		unsigned int m_VertexCount;
		ID3D11Buffer* m_BufferPointers[2];
		size_t m_InstanceCount;
	};

	template<typename V>
	inline VertexBuffer::VertexBuffer(std::vector<V>& data, std::vector<InstancePosition>* instanceData)
	{
		m_InstanceCount = 1;
		m_VertexCount = (unsigned int)std::size(data);

		D3D11_BUFFER_DESC vDesc = {};
		vDesc.Usage = D3D11_USAGE_DEFAULT;
		vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vDesc.ByteWidth = (unsigned int)(sizeof(V) * std::size(data));

		D3D11_SUBRESOURCE_DATA vData = {};
		vData.pSysMem = data.data();

		RendererContext::GetDevice()->CreateBuffer(&vDesc, &vData, &m_VertexBuffer);

		if (instanceData) 
		{
			m_InstanceCount = instanceData->size();

			D3D11_BUFFER_DESC iDesc = {};
			iDesc.Usage = D3D11_USAGE_DEFAULT;
			iDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			iDesc.ByteWidth = (unsigned int)(sizeof(InstancePosition) * m_InstanceCount);

			D3D11_SUBRESOURCE_DATA iData = {};
			iData.pSysMem = instanceData->data();
			RendererContext::GetDevice()->CreateBuffer(&iDesc, &iData, &m_InstanceBuffer);
		}
		m_Strides[0] = sizeof(V);
		m_Strides[1] = sizeof(InstancePosition);
		m_Offset[0] = 0;
		m_Offset[1] = 0;

		m_BufferPointers[0] = m_VertexBuffer.Get();
		m_BufferPointers[1] = m_InstanceBuffer.Get();
	}
}
