#include "Renderable/Utils/Node.h"


namespace Yassin
{
	Node::Node(aiNode* node, aiMatrix4x4 nodeTransform, aiMatrix4x4 parentAbsoluteTransform)
	{
		m_Name = node->mName.C_Str();
		m_RelativeTransform = DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&nodeTransform));
		m_ParentAbsoluteTransform = DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&parentAbsoluteTransform));
	}
}