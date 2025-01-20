#pragma once
#include "Renderer/Renderable.h"
#include <unordered_map>

namespace Yassin
{
	enum class CBufferType
	{
		PS,
		VS,
	};

	struct TextureMetaData
	{
		LPCSTR name;
		unsigned int slot;
	};

	struct SamplerMetaData
	{
		LPCSTR name;
		unsigned int slot;
	};

	class Material
	{
	public:
		inline void SetLightFlag(bool flag) { m_LightFlag = flag; }
		inline void SetTransparencyFlag(bool flag) { m_TransparencyFlag = flag; }
		inline void SetName(const char* name) { m_Name = name; }
		inline std::string GetName() const { return m_Name; }

		inline bool IsIlluminated() const { return m_LightFlag; }
		inline bool IsTransparent() const { return m_TransparencyFlag; }

		inline void AddTexture(LPCSTR name, unsigned int slot)
		{
			m_TextureMetaData.push_back({ name, slot });
		}

		inline void AddSampler(LPCSTR name, unsigned int slot)
		{
			m_SamplerMetaData.push_back({ name, slot });
		}

		inline const std::vector<TextureMetaData>& GetTextures() { return m_TextureMetaData; }
		inline const std::vector<SamplerMetaData>& GetSamplers() { return m_SamplerMetaData; }

		inline void SetVertexShader(VertexShader* vert)
		{
			m_VertexShader = vert;
		}

		inline void SetPixelShader(PixelShader* pixel)
		{
			m_PixelShader = pixel;
		}

		inline VertexShader* GetVertexShader() const { return m_VertexShader; }
		inline PixelShader* GetPixelShader() const { return m_PixelShader; }

	private:
		bool m_LightFlag = false;
		bool m_TransparencyFlag = false;
		std::vector<TextureMetaData> m_TextureMetaData;
		std::vector<SamplerMetaData> m_SamplerMetaData;
		VertexShader* m_VertexShader;
		PixelShader* m_PixelShader;
		std::string m_Name;
	};
}
