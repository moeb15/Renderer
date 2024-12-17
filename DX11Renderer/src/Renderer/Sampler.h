#pragma once
#include "Renderer/RendererContext.h"

namespace Yassin
{
	enum class FilterType
	{
		Anisotropic,
		Bilinear,
		Point,
	};

	enum class AddressType
	{
		Clamp,
		Mirror,
		Wrap
	};
	class Sampler
	{
	public:
		Sampler() = default;

		void Init(FilterType fType, AddressType aType);

		inline void Bind(unsigned int slot) 
		{
			RendererContext::GetDeviceContext()->PSSetSamplers(slot, 1, m_Sampler.GetAddressOf());
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_Sampler;
	};
}
