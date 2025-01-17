#include "Renderer/Resources/MaterialSystem.h"
#include <d3d11shader.h>

namespace Yassin 
{
	MaterialSystem* MaterialSystem::s_Instance = nullptr;

	MaterialSystem::MaterialSystem()
	{
		assert(s_Instance == nullptr);
		s_Instance = this;
	}

	void MaterialSystem::Init()
	{
		s_Instance->m_Materials.clear();
	}

	void MaterialSystem::Add(const std::string& name, std::pair<VertexShader*, PixelShader*> shaderPair)
	{
		if(s_Instance->m_Materials.find(name) != s_Instance->m_Materials.end()) 
		{
			std::string message = "Material " + name + " already exists";
			MessageBoxA(RendererContext::GetWindowHandle(), message.c_str(), "Material System", MB_OK);
			return;
		}

		HRESULT hr;
		ID3DBlob* vertexBlob = shaderPair.first->GetBlob();
		ID3DBlob* pixelBlob = shaderPair.second->GetBlob();

		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> pVertexRef = nullptr;
		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> pPixelRef = nullptr;

		hr = D3DReflect(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &pVertexRef);
		if(FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create ID3D11ShaderReflection from vs blob", L"Material System | Vertex Shader", MB_OK);
			return;
		}

		hr = D3DReflect(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &pPixelRef);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create ID3D11ShaderReflection from ps blob", L"Material System | Pixel Shader", MB_OK);
			return;
		}
		
		std::unique_ptr<Material> newMaterial = std::make_unique<Material>();

		D3D11_SHADER_DESC pixelShaderDesc = {};
		D3D11_SHADER_DESC vertexShaderDesc = {};

		pVertexRef->GetDesc(&vertexShaderDesc);
		pPixelRef->GetDesc(&pixelShaderDesc);

		// Will set a flag in Material when more than 1 CBuffers are present in VS or PS shader,
		// signifying that LightPositionBuffer and LightPropertiesBuffer are present, each at 
		// slots 1 and 0 respectively 

		newMaterial->SetLightFlag(pixelShaderDesc.ConstantBuffers >= 1 || vertexShaderDesc.ConstantBuffers > 1);
		newMaterial->SetTransparencyFlag(pixelShaderDesc.ConstantBuffers >= 2);

		// iterate over shader resources
		for(unsigned int i = 0; i < pixelShaderDesc.BoundResources; i++)
		{
			D3D11_SHADER_INPUT_BIND_DESC iDesc = {};
			pPixelRef->GetResourceBindingDesc(i, &iDesc);
			if (iDesc.Type == D3D_SIT_CBUFFER) continue;
			

			switch(iDesc.Type)
			{
				case D3D_SIT_TEXTURE:
					newMaterial->AddTexture(iDesc.Name, iDesc.BindPoint);
					break;

				case D3D_SIT_SAMPLER:
					newMaterial->AddSampler(iDesc.Name, iDesc.BindPoint);
					break;
			}
		}
		
		newMaterial->SetVertexShader(shaderPair.first);
		newMaterial->SetPixelShader(shaderPair.second);

		s_Instance->m_Materials.emplace(std::pair<std::string, std::unique_ptr<Material>>(name, std::move(newMaterial)));
	}

	Material* MaterialSystem::Get(const std::string& name)
	{
		if (s_Instance->m_Materials.find(name) == s_Instance->m_Materials.end())
		{
			return s_Instance->m_Materials.find("Error Material")->second.get();
		}
		const auto& kvPair = s_Instance->m_Materials.find(name);
		return kvPair->second.get();
	}
}