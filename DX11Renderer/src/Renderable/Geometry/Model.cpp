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
		const aiScene* pScene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
		if (!pScene) return;
		m_RootTransform = pScene->mRootNode->mTransformation;
		ProcessNode(pScene->mRootNode, pScene, material, world, instancePositions);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene, std::string material, DirectX::XMMATRIX world, std::vector<InstancePosition>* instancePositions)
	{
		for(unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* pMesh = scene->mMeshes[node->mMeshes[i]];
			ProcessMesh(node, pMesh, scene, material, world, instancePositions);
		}

		for(unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, material, world, instancePositions);
		}
	}

	void Model::ProcessMesh(aiNode* node, aiMesh* mesh, const aiScene* scene, std::string material, DirectX::XMMATRIX world, std::vector<InstancePosition>* instancePositions)
	{
		std::vector<Vertex> vData = {};
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex v;
			aiVector3D transformedPos = node->mTransformation * mesh->mVertices[i];
			v.position = *reinterpret_cast<DirectX::XMFLOAT3*>(&transformedPos);
			v.normal = *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh->mNormals[i]);

			if (mesh->mTextureCoords[0])
			{
				v.uv.x = (float)mesh->mTextureCoords[0]->x;
				v.uv.y = (float)mesh->mTextureCoords[0]->y;
			}
			else
			{
				v.uv = DirectX::XMFLOAT2(0.0f, 0.0f);
			}

			vData.push_back(v);
		}

		std::vector<unsigned long> indices = {};
		indices.reserve(mesh->mNumFaces * 3);
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const auto& face = mesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		m_Meshes.push_back(std::make_unique<Mesh>(material, world, vData, indices, instancePositions));
	}

	void Model::Render(DirectX::XMMATRIX& viewProj, bool bIgnoreMaterial) const
	{
		for(int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->Render(viewProj, bIgnoreMaterial);
		}
	}

	void Model::UpdateLighting(const LightPositionBuffer& lPos, const LightPropertiesBuffer& lProps) const
	{
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->UpdateLighting(lPos, lProps);
		}
	}

	MaterialInstance* Model::GetMaterialInstance(unsigned int meshIdx) const
	{
		if (meshIdx > m_Meshes.size() - 1) return nullptr;

		return m_Meshes[meshIdx]->GetMaterialInstance();
	}

	VertexBuffer* Model::GetVertexBuffer(unsigned int meshIdx) const
	{
		if (meshIdx > m_Meshes.size() - 1) return nullptr;
	
		return m_Meshes[meshIdx]->GetVertexBuffer();
	}
	
	IndexBuffer* Model::GetIndexBuffer(unsigned int meshIdx) const
	{
		if (meshIdx > m_Meshes.size() - 1) return nullptr;

		return m_Meshes[meshIdx]->GetIndexBuffer();
	}
	
	TransformBuffer* Model::GetTransformBuffer(unsigned int meshIdx)
	{
		if (meshIdx > m_Meshes.size() - 1) return nullptr;

		return m_Meshes[meshIdx]->GetTransformBuffer();
	}

	void Model::Translate(float x, float y, float z)
	{
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->Translate(x, y, z);
		}
	}

	void Model::Rotate(float yaw, float pitch, float roll)
	{
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->Rotate(yaw, pitch, roll);
		}
	}

	void Model::Scale(float x, float y, float z)
	{
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->Scale(x, y, z);
		}
	}
}
