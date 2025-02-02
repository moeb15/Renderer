#pragma once

namespace Resources
{
	enum class TextureType
	{
		Error,
		Stone,
		Metal,
	};

	enum class ShaderType
	{
		Test,
		Texture,
		UnlitTexture,
		Depth,
		ShadowMap,
		GaussianBlur,
		BoxBlur,
	};

	enum class MaterialType
	{
		Error,
		Texture,
		Depth,
		ShadowMap,
	};
}