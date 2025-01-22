#pragma once
#include <assimp/scene.h>
#include <DirectXMath.h>

namespace Yassin
{
	class Node
	{
	public:
		Node(aiNode* node, aiMatrix4x4 nodeTransform, aiMatrix4x4 parentAbsoluteTransform);

	private:
		std::string m_Name;
		DirectX::XMMATRIX m_ParentAbsoluteTransform;
		DirectX::XMMATRIX m_RelativeTransform;
	};
}

