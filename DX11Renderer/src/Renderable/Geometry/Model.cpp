#include "Renderable/Geometry/Model.h"
#include "Renderer/Resources/MaterialSystem.h"
#include "Renderer/Resources/TextureLibrary.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "imgui.h"
#include "Model.h"

constexpr float negInf = -std::numeric_limits<float>::infinity();
constexpr float posInf = std::numeric_limits<float>::infinity();

namespace Yassin
{
	Model::Model(std::string material, const std::string& modelFile, DirectX::XMMATRIX world, std::vector<InstancePosition>* instancePositions)
	{
		Assimp::Importer importer;
		const aiScene* pScene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded);
		if (!pScene) 
		{
			MessageBoxA(RendererContext::GetWindowHandle(), "Failed to load model", modelFile.c_str(), MB_OK);
			exit(-1);
		}
		m_RootTransform = pScene->mRootNode->mTransformation;

		size_t lastSlash = modelFile.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		m_ModelDirectory = modelFile.substr(0, lastSlash) + "Textures/";
		ProcessNode(pScene->mRootNode, pScene, material, world, instancePositions, m_RootTransform);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene, std::string material, DirectX::XMMATRIX world, std::vector<InstancePosition>* instancePositions, aiMatrix4x4 parentTransform)
	{
		for(unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* pMesh = scene->mMeshes[node->mMeshes[i]];
			ProcessMesh(node, pMesh, scene, material, world, instancePositions, parentTransform);
		}

		for(unsigned int i = 0; i < node->mNumChildren; i++)
		{
			aiMatrix4x4 childTransform = node->mChildren[i]->mTransformation;
			ProcessNode(node->mChildren[i], scene, material, world, instancePositions, parentTransform * childTransform);
		}
	}

	void Model::ProcessMesh(aiNode* node, aiMesh* mesh, const aiScene* scene, std::string material, DirectX::XMMATRIX world, std::vector<InstancePosition>* instancePositions, aiMatrix4x4 nodeTransform)
	{
		float meshBoundsMaxX = negInf;
		float meshBoundsMaxY = negInf;
		float meshBoundsMaxZ = negInf;

		float meshBoundsMinX = posInf;
		float meshBoundsMinY = posInf;
		float meshBoundsMinZ = posInf;

		float meshBoundsX;
		float meshBoundsY;
		float meshBoundsZ;
		
		std::vector<Vertex> vData = {};
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex v;
			aiVector3D prePos = mesh->mVertices[i];
			aiVector3D transformedPos = nodeTransform * mesh->mVertices[i];
			aiVector3D normal = mesh->mNormals[i];
			aiVector3D tangent = mesh->mTangents[i];
			aiVector3D binormal = mesh->mBitangents[i];
			v.position = *reinterpret_cast<DirectX::XMFLOAT3*>(&transformedPos);
			v.normal = *reinterpret_cast<DirectX::XMFLOAT3*>(&normal);
			v.tangent = *reinterpret_cast<DirectX::XMFLOAT3*>(&tangent);
			v.binormal = *reinterpret_cast<DirectX::XMFLOAT3*>(&binormal);
			
			if (mesh->mTextureCoords[0])
			{
				v.uv.x = (float)mesh->mTextureCoords[0][i].x;
				v.uv.y = (float)mesh->mTextureCoords[0][i].y;
			}
			else
			{
				v.uv.x = 0.0f;
				v.uv.y = 0.0f;
			}

			if (meshBoundsMinX > transformedPos.x) meshBoundsMinX = transformedPos.x;
			if (meshBoundsMinY > transformedPos.y) meshBoundsMinY = transformedPos.y;
			if (meshBoundsMinZ > transformedPos.z) meshBoundsMinZ = transformedPos.z;

			if (meshBoundsMaxX < transformedPos.x) meshBoundsMaxX = transformedPos.x;
			if (meshBoundsMaxY < transformedPos.y) meshBoundsMaxY = transformedPos.y;
			if (meshBoundsMaxZ < transformedPos.z) meshBoundsMaxZ = transformedPos.z;

			vData.push_back(v);
		}

		meshBoundsX = abs(meshBoundsMaxX - meshBoundsMinX);
		meshBoundsY = abs(meshBoundsMaxY - meshBoundsMinY);
		meshBoundsZ = abs(meshBoundsMaxZ - meshBoundsMinZ);

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
		
		aiMaterial* pMaterial = scene->mMaterials[mesh->mMaterialIndex];
		std::string diffuseTex;
		std::string normalTex;
		std::string specularTex;
		std::string roughnessTex;
		diffuseTex = LoadMaterialTextures(pMaterial, aiTextureType::aiTextureType_DIFFUSE, scene);
		normalTex = LoadMaterialTextures(pMaterial, aiTextureType::aiTextureType_NORMALS, scene);
		if (normalTex == "") normalTex = LoadMaterialTextures(pMaterial, aiTextureType::aiTextureType_HEIGHT, scene);
		specularTex = LoadMaterialTextures(pMaterial, aiTextureType::aiTextureType_SPECULAR, scene);
		roughnessTex = LoadMaterialTextures(pMaterial, aiTextureType::aiTextureType_UNKNOWN, scene);

		DirectX::XMMATRIX meshTransform = DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&nodeTransform));

		m_Meshes.push_back(std::make_unique<Mesh>(material, world, meshTransform, vData, indices, meshBoundsX, meshBoundsY, meshBoundsZ, node->mName.C_Str(), instancePositions));

		if (diffuseTex != "") m_Meshes.back()->SetTexture(TextureSlot::BaseTexture, diffuseTex);
		if (normalTex != "") m_Meshes.back()->SetTexture(TextureSlot::NormalTexture, normalTex);
		if (specularTex != "") m_Meshes.back()->SetTexture(TextureSlot::SpecularTexture, specularTex);
		if (roughnessTex != "") m_Meshes.back()->SetTexture(TextureSlot::RoughnessTexture, roughnessTex);
	}

	std::string Model::LoadMaterialTextures(aiMaterial* material, aiTextureType texType, const aiScene* scene)
	{
		unsigned int texCount = material->GetTextureCount(texType);
		std::string texName = "";
		aiString path;
		if (material->GetTexture(texType, 0, &path) == aiReturn_SUCCESS)
		{
			std::string sPath;
			sPath = path.C_Str();
			if (sPath.find('.') == std::string::npos) return texName;
			std::string fileName = m_ModelDirectory + sPath;
			size_t nameLen = sPath.rfind('.');
			texName = sPath.substr(0, nameLen);
			TextureLibrary::Add(texName, fileName, TextureType::Tex2D);
		}

		return texName;
	}

	void Model::Render(Camera& camera, DirectX::XMMATRIX& viewProj, bool bIgnoreMaterial, bool bRenderBoundingVolume, bool bIgnoreCulling)
	{
		for(int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->Render(camera, viewProj, bIgnoreMaterial, bRenderBoundingVolume, bIgnoreCulling);
		}
	}

	void Model::UpdateLighting(const LightPositionBuffer& lPos, const LightPropertiesBuffer& lProps) const
	{
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->UpdateLighting(lPos, lProps);
		}
	}

	void Model::UpdateCameraPosition(const CameraPositionType& cPos) const
	{
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->UpdateCameraPosition(cPos);
		}
	}

	void Model::UpdateLightDirection(const LightDirectionType& lDir) const
	{
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->UpdateLightDirection(lDir);
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

	const size_t Model::GetCulledCount() const
	{
		size_t meshesCulled = 0;
		for(int i = 0; i < m_Meshes.size(); i++)
		{
			meshesCulled += m_Meshes[i]->GetCulledCount();
		}
		return meshesCulled;
	}

	const size_t Model::GetMeshCount() const
	{
		return m_Meshes.size();
	}

	void Model::ResetCulledCount()
	{
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->ResetCulledCount();
		}
	}

	void Model::SetMaterial(std::string material)
	{
		for(int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->SetMaterial(material);
		}
	}

	void Model::BindShaderResources()
	{
		for(int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i]->BindShaderResources();
		}
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
