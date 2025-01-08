#pragma once
#include "Renderer/Primitives/Texture.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace Yassin
{
	enum class TextureType
	{
		Tex1D,
		Tex2D,
		Tex3D,
	};
	class TextureLibrary
	{
	public:
		TextureLibrary();

		static void Init();
		static void Add(const std::string& name, const std::string& textureFile, TextureType type);
		static Texture2D* GetTexture2D(const std::string& name);

	private:
		static TextureLibrary* s_Instance;
		std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_Texture2DMap;
	};
}
