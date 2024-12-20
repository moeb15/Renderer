#include "Renderer/MaterialSystem.h"
#include "MaterialSystem.h"
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

	// TODO : Need to record input layout of vertex shader to allow for creation of input layout object in material instances
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

		// iterate over constant buffers, slot 0 is reserved for transform buffer
		for(int i = 1; i < vertexShaderDesc.ConstantBuffers; i++)
		{
			D3D11_SHADER_BUFFER_DESC bufferDesc = {};
			ID3D11ShaderReflectionConstantBuffer* cBuffer = pVertexRef->GetConstantBufferByIndex(i);
			cBuffer->GetDesc(&bufferDesc);

			CBufferMetaData cBufferData;
			cBufferData.type = CBufferType::VS;
			cBufferData.name = bufferDesc.Name;
			cBufferData.size = bufferDesc.Size;
			cBufferData.slot = i;

			for(int j = 0; j < bufferDesc.Variables; j++)
			{
				ID3D11ShaderReflectionVariable* var = cBuffer->GetVariableByIndex(j);
				ID3D11ShaderReflectionType* type = var->GetType();
				D3D11_SHADER_VARIABLE_DESC varDesc = {};
				D3D11_SHADER_TYPE_DESC tDesc = {};
				type->GetDesc(&tDesc);
				var->GetDesc(&varDesc);

				CBufferVariableData cbufVar;
				cbufVar.name = varDesc.Name;
				cbufVar.type = tDesc.Type;
				cbufVar.size = varDesc.Size;

				cBufferData.variables.push_back(cbufVar);
			}
			newMaterial->AddCBuffer(cBufferData);
		}

		// iterate over constant buffers, slot 0 is reserved for light buffer
		for (int i = 1; i < pixelShaderDesc.ConstantBuffers; i++)
		{
			D3D11_SHADER_BUFFER_DESC bufferDesc = {};
			ID3D11ShaderReflectionConstantBuffer* cBuffer = pPixelRef->GetConstantBufferByIndex(i);
			cBuffer->GetDesc(&bufferDesc);

			CBufferMetaData cBufferData;
			cBufferData.type = CBufferType::PS;
			cBufferData.name = bufferDesc.Name;
			cBufferData.size = bufferDesc.Size;
			cBufferData.slot = i;

			for (int j = 0; j < bufferDesc.Variables; j++)
			{
				ID3D11ShaderReflectionVariable* var = cBuffer->GetVariableByIndex(j);
				ID3D11ShaderReflectionType* type = var->GetType();
				D3D11_SHADER_VARIABLE_DESC varDesc = {};
				D3D11_SHADER_TYPE_DESC tDesc = {};
				type->GetDesc(&tDesc);
				var->GetDesc(&varDesc);

				CBufferVariableData cbufVar;
				cbufVar.name = varDesc.Name;
				cbufVar.type = tDesc.Type;
				cbufVar.size = varDesc.Size;

				cBufferData.variables.push_back(cbufVar);
			}
			newMaterial->AddCBuffer(cBufferData);
		}

		// iterate over shader resources
		for(int i = 0; i < pixelShaderDesc.BoundResources; i++)
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