#pragma once

namespace Yassin
{
	enum TextureSlot
	{
		BaseTexture = 0,
		DepthMapTexture,
	};

	enum VSBufferSlot
	{
		Matrix = 0,
		LightPosition,
	};

	enum PSBufferSlot
	{
		LightProperties = 0,
		Transparency,
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

	enum BlurType
	{
		Gaussian,
	};
}