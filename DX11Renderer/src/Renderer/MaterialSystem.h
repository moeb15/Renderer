#pragma once
#include "Renderer/PixelShader.h"
#include "Renderer/VertexShader.h"
#include "Renderer/Material.h"

namespace Yassin
{
	class MaterialSystem
	{
	public:
		MaterialSystem();
		static void Init();
		static void Add(const std::string& name, std::pair<VertexShader*, PixelShader*> shaderPair);
		static Material* Get(const std::string& name);

	private:
		static MaterialSystem* s_Instance;
		std::unordered_map<std::string, std::unique_ptr<Material>> m_Materials;
	};
}
