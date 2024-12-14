#pragma once
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexShader.h"
#include "Renderer/PixelShader.h"
#include "Renderer/InputLayout.h"
#include "Renderer/Topology.h"
#include "Renderer/ConstantBuffer.h"

#include "Renderer/ShaderLibrary.h"

namespace Yassin
{
	class Renderable
	{
	public:
		//virtual VertexBuffer* GetVertexBuffer() const = 0;
		//virtual IndexBuffer* GetIndexBuffer() const = 0;
		//virtual VertexShader* GetVertexShader() const = 0;
		//virtual PixelShader* GetPixelShader() const = 0;
		//virtual Topology* GetTopology() const = 0;
		//virtual InputLayout* GetInputLayout() const = 0;
		//virtual TransformBuffer* GetTransformBuffer() const = 0;

		virtual void Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) const = 0;
	};
}
