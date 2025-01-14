#include "Renderable/Geometry/Model.h"
#include "Renderer/Resources/MaterialSystem.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Yassin
{
	Model::Model(std::string material, const std::string& modelFile, DirectX::XMMATRIX world, std::vector<InstancePosition>* instancePositions)
	{
		Assimp::Importer importer;
		const aiScene* pModel = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
		const aiMesh* pMesh = pModel->mMeshes[0];

		std::vector<Vertex> vData;
		for(unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			Vertex v;
			v.position = *reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mVertices[i]);
			if (pMesh->mTextureCoords[0] != nullptr) 
			{
				v.uv.x = reinterpret_cast<DirectX::XMFLOAT3*>(pMesh->mTextureCoords[0])->x;
				v.uv.y = reinterpret_cast<DirectX::XMFLOAT3*>(pMesh->mTextureCoords[0])->y;
			}
			v.normal = *reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mNormals[i]);

			vData.push_back(v);
		}

		std::vector<unsigned long> indices;
		indices.reserve(pMesh->mNumFaces * 3);
		for(unsigned int i = 0 ; i < pMesh->mNumFaces; i++)
		{
			const auto& face = pMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		m_InstancedDraw = instancePositions != nullptr;

		m_VertexBuffer = std::make_unique<VertexBuffer>(vData, instancePositions);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices);

		m_TransformBuffer = std::make_unique<TransformBuffer>();
		m_TransformBuffer->SetWorld(world);

		m_Material = std::make_unique<MaterialInstance>(MaterialSystem::Get(material));
		m_Material->SetTexture(TextureSlot::BaseTexture, "ErrorTexture");
		m_Material->SetSampler(0, FilterType::Bilinear, AddressType::Clamp);
		m_Material->SetSampler(1, FilterType::Bilinear, AddressType::Wrap);
	}
}
