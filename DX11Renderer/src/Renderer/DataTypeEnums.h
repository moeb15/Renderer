#pragma once

namespace Yassin
{
	enum TextureSlot
	{
		BaseTexture = 0,
		NormalTexture,
		RoughnessTexture,
		DepthMapTexture,
	};

	enum SamplerSlot
	{
		ClampSampler = 0,
		WrapSampler,
	};

	enum VSBufferSlot
	{
		Matrix = 0,
		LightPosition,
		CameraPosition,
	};

	enum PSBufferSlot
	{
		LightProperties = 0,
		Transparency,
		LightDirection,
	};

	enum class ObjectType
	{
		Geometry,
		Light,
	};

	enum class ObjectVisibility
	{
		Opaque,
		Transparent,
	};

	enum RenderTargetType
	{
		DepthMap,
		DepthStencil,
	};

	enum class BlurType
	{
		GaussianBlur,
		BoxBlur,
	};
}