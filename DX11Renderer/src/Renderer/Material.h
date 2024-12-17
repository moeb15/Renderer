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

	struct CBufferVariableData
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
		void AddCBuffer(const CBufferMetaData& cbuffer)
		{
			m_CbufferMetaData.push_back(cbuffer);
		}

		void AddTexture(LPCSTR name, unsigned int slot)
		{
			m_TextureMetaData.push_back({ name, slot });
		}

		void AddSampler(LPCSTR name, unsigned int slot)
		{
			m_SamplerMetaData.push_back({ name, slot });
		}

		const std::vector<CBufferMetaData>& GetCBuffers() { return m_CbufferMetaData; }
		const std::vector<TextureMetaData>& GetTextures() { return m_TextureMetaData; }
		const std::vector<SamplerMetaData>& GetSamplers() { return m_SamplerMetaData; }

		void SetVertexShader(VertexShader* vert)
		{
			m_VertexShader = vert;
		}

		void SetPixelShader(PixelShader* pixel)
		{
			m_PixelShader = pixel;
		}

		VertexShader* GetVertexShader() const { return m_VertexShader; }
		PixelShader* GetPixelShader() const { return m_PixelShader; }

	private:
		std::vector<CBufferMetaData> m_CbufferMetaData;
		std::vector<TextureMetaData> m_TextureMetaData;
		std::vector<SamplerMetaData> m_SamplerMetaData;

		VertexShader* m_VertexShader;
		PixelShader* m_PixelShader;
	};
}
