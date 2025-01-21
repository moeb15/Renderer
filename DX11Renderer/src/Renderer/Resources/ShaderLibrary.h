#pragma once
#include "Renderer/Primitives/VertexShader.h"
#include "Renderer/Primitives/PixelShader.h"
#include "Renderer/Primitives/ComputeShader.h"
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

		static void AddCompute(const std::string& computeShaderName, wchar_t* csFile);
		static ComputeShader* GetCompute(const std::string& computeShaderName);

	private:
		static ShaderLibrary* s_Instance;
		std::unordered_map<std::string, UniqueShaderPair> m_ShaderPairs;
		std::unordered_map<std::string, std::unique_ptr<ComputeShader>> m_ComputeShaders;
	};
}
