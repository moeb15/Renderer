#pragma once
#include "Renderer/Renderable.h"
#include <unordered_map>

// TODO : Need to record input layout of vertex shader to allow for creation of input layout object in material instances
namespace Yassin
{
	enum class CBufferType
	{
		PS,
		VS,
	};

	// See changes in MaterialSystem.cpp as to why CBuffers have been removed
	// from Material.h
	/*struct CBufferVariableData
	{
		LPCSTR name;
		D3D_SHADER_VARIABLE_TYPE type;
		unsigned int size;
	};

	struct CBufferMetaData
	{
		CBufferType type;
		LPCSTR name;
		unsigned int size;
		unsigned int slot;
		std::vector<CBufferVariableData> variables;
	};*/

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

		inline bool IsIlluminated() const { return m_LightFlag; }

		inline void AddTexture(LPCSTR name, unsigned int slot)
		{
			m_TextureMetaData.push_back({ name, slot });
		}

		inline void AddSampler(LPCSTR name, unsigned int slot)
		{
			m_SamplerMetaData.push_back({ name, slot });
		}

		//inline const std::vector<CBufferMetaData>& GetCBuffers() { return m_CbufferMetaData; }
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
		//std::vector<CBufferMetaData> m_CbufferMetaData;
		bool m_LightFlag = false;
		std::vector<TextureMetaData> m_TextureMetaData;
		std::vector<SamplerMetaData> m_SamplerMetaData;
		VertexShader* m_VertexShader;
		PixelShader* m_PixelShader;
	};
}
