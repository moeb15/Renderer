#pragma once
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexShader.h"
#include "Renderer/PixelShader.h"
#include "Renderer/InputLayout.h"
#include "Renderer/Topology.h"

namespace Yassin
{
	class Renderable
	{
	public:
		virtual VertexBuffer* GetVertexBuffer() const = 0;
		virtual IndexBuffer* GetIndexBuffer() const = 0;
		virtual VertexShader* GetVertexShader() const = 0;
		virtual PixelShader* GetPixelShader() const = 0;
		virtual Topology* GetTopology() const = 0;
		virtual InputLayout* GetInputLayout() const = 0;

		void Render();
	};
}
