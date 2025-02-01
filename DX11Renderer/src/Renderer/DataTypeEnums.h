#pragma once

namespace Yassin
{
	enum TextureSlot
	{
		BaseTexture = 0,
		NormalTexture,
		SpecularTexture,
		RoughnessTexture,
		ShadowMapTexture,
		DepthMapTexture,
		AmbientMap,
		TEXTURE_COUNT
	};

	enum SamplerSlot
	{
		ClampSampler = 0,
		WrapSampler,
		PointSampler,
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
		PointLights,
	};

	enum CSBufferSlot
	{
		CameraDataCB = 0,
		WorkGroupDataCB,
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