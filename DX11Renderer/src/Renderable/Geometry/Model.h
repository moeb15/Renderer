#pragma once
#include "Renderer/Renderable.h"
#include "Renderable/Utils/Mesh.h"
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Yassin
{
	class Model : public Renderable
	{
	public:
		Model(std::string material, const std::string& modelFile, DirectX::XMMATRIX world, std::vector<InstancePosition>* instancePositions = nullptr);
		void ProcessNode(aiNode* node, const aiScene* scene, std::string material, DirectX::XMMATRIX world, std::vector<InstancePosition>* instancePositions, aiMatrix4x4 parentTransform);
		void ProcessMesh(aiNode* node, aiMesh* mesh, const aiScene* scene, std::string material, DirectX::XMMATRIX world, std::vector<InstancePosition>* instancePositions, aiMatrix4x4 nodeTransform);
		std::string LoadMaterialTextures(aiMaterial* material, aiTextureType texType, const aiScene* scene);

		virtual void Render(Camera& camera, DirectX::XMMATRIX& viewProj, bool bIgnoreMaterial = false, bool bRenderBoundingVolume = false, bool bIgnoreCulling = false) override;
		virtual void UpdateLighting(const LightPositionBuffer& lPos, const LightPropertiesBuffer& lProps) const override;
		virtual void UpdateCameraPosition(const CameraPositionType& cPos) const override;
		virtual void UpdateLightDirection(const LightDirectionType& lDir) const override;

		virtual MaterialInstance* GetMaterialInstance(unsigned int meshIdx = 0) const override;
		virtual VertexBuffer* GetVertexBuffer(unsigned int meshIdx = 0) const override;
		virtual IndexBuffer* GetIndexBuffer(unsigned int meshIdx = 0) const override;
		virtual TransformBuffer* GetTransformBuffer(unsigned int meshIdx = 0) override;
		virtual const size_t GetCulledCount() const override;
		virtual const size_t GetMeshCount() const override;

		virtual void ResetCulledCount() override;
		virtual void SetMaterial(std::string material) override;
		virtual void SetTexture(unsigned int slot, const std::string& texture) {}
		virtual void BindShaderResources() override;


		virtual void Translate(float x, float y, float z) override;
		virtual void Rotate(float yaw, float pitch, float roll) override;
		virtual void Scale(float x, float y, float z) override;

	private:
		std::vector<std::unique_ptr<Mesh>> m_Meshes;
		std::string m_ModelDirectory;
		aiMatrix4x4 m_RootTransform;
	};
}
