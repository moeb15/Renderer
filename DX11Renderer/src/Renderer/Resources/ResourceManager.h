#pragma once
#include "Renderer/Resources/MaterialSystem.h"
#include "Renderer/Resources/TextureLibrary.h"
#include "Renderer/Resources/ShaderLibrary.h"
#include <future>
#include <atomic>

namespace Yassin
{
	class ResourceManager
	{
	public:
		void Init();
		bool IsLoadingComplete() const;

	private:
		void LoadResources();

	private:
		std::future<void> m_LoadTask;
		std::atomic<bool> m_IsLoadingComplete{ false };

		std::unique_ptr<TextureLibrary> m_TextureLibrary;
		std::unique_ptr<MaterialSystem> m_MaterialSystem;
		std::unique_ptr<ShaderLibrary> m_ShaderLibrary;
	};
}
