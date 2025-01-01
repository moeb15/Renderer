#pragma once

namespace Yassin
{
	enum TextureSlot
	{
		BaseTexture = 0,
		DepthMapTexture,
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
}