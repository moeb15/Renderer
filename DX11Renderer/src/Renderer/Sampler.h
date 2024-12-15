#pragma once
#include "Renderer/RendererContext.h"

namespace Yassin
{
	class Sampler
	{
	public:
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

	public:
		Sampler(FilterType fType, AddressType aType);

		inline void Bind(unsigned int slot) 
		{
			RendererContext::GetDeviceContext()->PSSetSamplers(slot, 1, m_Sampler.GetAddressOf());
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_Sampler;
	};
}
