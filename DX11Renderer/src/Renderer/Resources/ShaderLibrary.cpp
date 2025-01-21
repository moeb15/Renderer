#include "Renderer/Resources/ShaderLibrary.h"
#include "ShaderLibrary.h"

namespace Yassin
{
	ShaderLibrary* ShaderLibrary::s_Instance = nullptr;

	ShaderLibrary::ShaderLibrary()
	{
		assert(s_Instance == nullptr);
		s_Instance = this;
	}

	void ShaderLibrary::Init()
	{
		s_Instance->m_ShaderPairs.clear();
	}

	void ShaderLibrary::Add(const std::string& shaderName, wchar_t* vsFile, wchar_t* psFile)
	{
		if (s_Instance->m_ShaderPairs.find(shaderName) != s_Instance->m_ShaderPairs.end()) return;

		std::unique_ptr<VertexShader> vShader = std::make_unique<VertexShader>(vsFile);
		if (!vShader) return;

		std::unique_ptr<PixelShader> pShader = std::make_unique<PixelShader>(psFile);
		if (!vShader) return;

		s_Instance->m_ShaderPairs.emplace(shaderName, UniqueShaderPair(std::move(vShader), std::move(pShader)));
	}

	ShaderLibrary::VsPsPair ShaderLibrary::Get(const std::string& shaderName)
	{
		if (s_Instance->m_ShaderPairs.find(shaderName) == s_Instance->m_ShaderPairs.end()) 
			return VsPsPair(nullptr, nullptr);

		auto kvPair = s_Instance->m_ShaderPairs.find(shaderName);
		VsPsPair shaderPair = VsPsPair(kvPair->second.first.get(), kvPair->second.second.get());
		return shaderPair;
	}

	void ShaderLibrary::AddCompute(const std::string& computeShaderName, wchar_t* csFile)
	{
		if (s_Instance->m_ComputeShaders.find(computeShaderName) != s_Instance->m_ComputeShaders.end()) return;

		std::unique_ptr<ComputeShader> computeShader = std::make_unique<ComputeShader>(csFile);
		if (!computeShader) return;

		s_Instance->m_ComputeShaders.emplace(computeShaderName, std::move(computeShader));
	}

	ComputeShader* Yassin::ShaderLibrary::GetCompute(const std::string& computeShaderName)
	{
		if (s_Instance->m_ComputeShaders.find(computeShaderName) == s_Instance->m_ComputeShaders.end()) return nullptr;

		auto kvPair = s_Instance->m_ComputeShaders.find(computeShaderName);
		return kvPair->second.get();
	}
}