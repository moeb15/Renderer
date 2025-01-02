#include "Renderer/Primitives/Sampler.h"


namespace Yassin
{
	void Sampler::Init(FilterType fType, AddressType aType)
	{
		HRESULT hr;
		D3D11_SAMPLER_DESC sDesc = {};

		switch (fType)
		{
		case FilterType::Anisotropic:
			sDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			break;

		case FilterType::Bilinear:
			sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			break;

		case FilterType::Point:
			sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		}

		switch (aType)
		{
		case AddressType::Clamp:
			sDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			sDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			sDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			break;

		case AddressType::Wrap:
			sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			break;

		case AddressType::Mirror:
			sDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
			sDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
			sDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
			break;
		}

		sDesc.MipLODBias = 0.0f;
		sDesc.MaxAnisotropy = 1;
		sDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sDesc.MinLOD = 0;
		sDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr = RendererContext::GetDevice()->CreateSamplerState(&sDesc, &m_Sampler);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create sampler", L"Sampler", MB_OK);
			return;
		}
	}
}