#pragma once
#include "Renderer/Primitives/VertexShader.h"
#include "Renderer/Primitives/PixelShader.h"
#include <string>
#include <utility>
#include <unordered_map>
#include <memory>

namespace Yassin
{
	class ShaderLibrary
	{
	public:
		typedef std::pair<std::unique_ptr<VertexShader>, std::unique_ptr<PixelShader>> UniqueShaderPair;
		typedef std::pair<VertexShader*, PixelShader*> VsPsPair;
	public:
		ShaderLibrary();
		ShaderLibrary(const ShaderLibrary&) = delete;
		ShaderLibrary& operator=(const ShaderLibrary&) = delete;

		static void Init();
		static void Add(const std::string& shaderName, wchar_t* vsFile, wchar_t* psFile);
		static VsPsPair Get(const std::string& shaderName);

	private:
		static ShaderLibrary* s_Instance;
		std::unordered_map<std::string, UniqueShaderPair> m_ShaderPairs;
	};
}
