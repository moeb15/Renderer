#include "Renderer/Resources/ResourceManager.h"
#include "ResourceManager.h"

namespace Yassin
{
	void ResourceManager::Init()
	{
		m_LoadTask = std::async(std::launch::async, &ResourceManager::LoadResources, this);
	}

	bool ResourceManager::IsLoadingComplete() const
	{
		return m_IsLoadingComplete;
	}

	void Yassin::ResourceManager::LoadResources()
	{
		m_ShaderLibrary = std::make_unique<ShaderLibrary>();
		m_MaterialSystem = std::make_unique<MaterialSystem>();
		m_TextureLibrary = std::make_unique<TextureLibrary>();

		TextureLibrary::Init();
		TextureLibrary::Add("Missing Texture", "src/Assets/Textures/ErrorTexture.png", TextureType::Tex2D);
		TextureLibrary::Add("Stone", "src/Assets/Textures/stone.png", TextureType::Tex2D);
		TextureLibrary::Add("Metal", "src/Assets/Textures/Metal.png", TextureType::Tex2D);

		ShaderLibrary::Init();
		ShaderLibrary::Add("Test Shader", L"src/Shaders/CSO/TestVS.cso", L"src/Shaders/CSO/TestPS.cso");
		ShaderLibrary::Add("Texture Shader", L"src/Shaders/CSO/TextureVS.cso", L"src/Shaders/CSO/TexturePS.cso");
		ShaderLibrary::Add("Unlit Texture Shader", L"src/Shaders/CSO/UnlitTextureVS.cso", L"src/Shaders/CSO/UnlitTexturePS.cso");
		ShaderLibrary::Add("Depth Shader", L"src/Shaders/CSO/DepthVS.cso", L"src/Shaders/CSO/DepthPS.cso");
		ShaderLibrary::Add("Shadow Map Shader", L"src/Shaders/CSO/ShadowMapVS.cso", L"src/Shaders/CSO/ShadowMapPS.cso");
		ShaderLibrary::Add("Gaussian Blur Shader", L"src/Shaders/CSO/DefaultVS.cso", L"src/Shaders/CSO/GaussianBlurPS.cso");
		ShaderLibrary::Add("Box Blur Shader", L"src/Shaders/CSO/DefaultVS.cso", L"src/Shaders/CSO/BoxBlurPS.cso");

		MaterialSystem::Init();
		MaterialSystem::Add("Error Material", ShaderLibrary::Get("Test Shader"));
		MaterialSystem::Add("Texture Material", ShaderLibrary::Get("Texture Shader"));
		MaterialSystem::Add("Depth Material", ShaderLibrary::Get("Depth Shader"));
		MaterialSystem::Add("Shadow Map Material", ShaderLibrary::Get("Shadow Map Shader"));

		m_IsLoadingComplete = true;
	}
}
