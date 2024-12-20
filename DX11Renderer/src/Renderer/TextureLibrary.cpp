#include "Renderer/TextureLibrary.h"

namespace Yassin
{
	TextureLibrary* TextureLibrary::s_Instance = nullptr;

	TextureLibrary::TextureLibrary()
	{
		assert(s_Instance == nullptr);
		s_Instance = this;
	}

	void TextureLibrary::Init()
	{
		s_Instance->m_Texture2DMap.clear();
	}

	void TextureLibrary::Add(const std::string& name, const std::string& textureFile, TextureType type)
	{
		bool success;
		switch(type)
		{
			case TextureType::Tex1D: 
				break;

			case TextureType::Tex2D:
				if (s_Instance->m_Texture2DMap.find(name) != s_Instance->m_Texture2DMap.end()) return;

				std::unique_ptr<Texture2D> tex2D = std::make_unique<Texture2D>();
				success = tex2D->Init(textureFile.c_str());
				if (!success) return;

				s_Instance->m_Texture2DMap.emplace(name, std::move(tex2D));
				break;
		}
	}

	Texture2D* TextureLibrary::GetTexture2D(const std::string& name)
	{
		if (s_Instance->m_Texture2DMap.find(name) == s_Instance->m_Texture2DMap.end()) 
		{
			// return missing texture instead
			return s_Instance->m_Texture2DMap.find("Missing Texture")->second.get();
		}

		return s_Instance->m_Texture2DMap.find(name)->second.get();
	}

}