#include "Renderable/Geometry/Box.h"
#include "Renderer/MaterialSystem.h"
#include "Box.h"


namespace Yassin
{
	Box::Box(std::string material, DirectX::XMMATRIX world) :
		m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		std::vector<Vertex> vData =
		{
			// front
			{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, {  0.0f,  0.0f, -1.0f } },
			{ { -0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f }, {  0.0f,  0.0f, -1.0f } },
			{ { 0.5f, 0.5f, -0.5f }, { 1.0f, 0.0f }, {  0.0f,  0.0f, -1.0f } },
			{ { 0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f } },

			// back
			{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f } },
			{ {  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f }, {  0.0f,  0.0f,  1.0f } },
			{ {  0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f }, {  0.0f,  0.0f,  1.0f } },
			{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, {  0.0f,  0.0f,  1.0f } },

			// left
		   { { -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f }, { -1.0f,  0.0f,  0.0f } },
		   { { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f }, { -1.0f,  0.0f,  0.0f } },
		   { { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f }, { -1.0f,  0.0f,  0.0f } },
		   { { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f } },

		   // right
		   { {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, {  1.0f,  0.0f,  0.0f } },
		   { {  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f }, {  1.0f,  0.0f,  0.0f } },
		   { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, {  1.0f,  0.0f,  0.0f } },
		   { {  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f } },

		   // top
		   { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f }, {  0.0f,  1.0f,  0.0f } },
		   { { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f }, {  0.0f,  1.0f,  0.0f } },
		   { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, {  0.0f,  1.0f,  0.0f } },
		   { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f } },

		   // bottom
		   { { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f } },
		   { {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, {  0.0f, -1.0f,  0.0f } },
		   { {  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f }, {  0.0f, -1.0f,  0.0f } },
		   { { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f }, {  0.0f, -1.0f,  0.0f } },
		};

		std::vector<unsigned long> indices =
		{
			// Front face
			0, 1, 2, 0, 2, 3,

			// Back face
			4, 5, 6, 4, 6, 7,

			// Left face
			8, 9, 10, 8, 10, 11,

			// Right face
			12, 13, 14, 12, 14, 15,

			// Top face
			16, 17, 18, 16, 18, 19,

			// Bottom face
			20, 21, 22, 20, 22, 23
		};

		m_VertexBuffer = std::make_unique<VertexBuffer>(vData);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices);

		m_TransformBuffer = std::make_unique<TransformBuffer>();
		m_TransformBuffer->SetWorld(world);

		m_Material = std::make_unique<MaterialInstance>(MaterialSystem::Get(material));
		m_Material->SetTexture(TextureSlot::BaseTexture, "Stone");
		m_Material->SetSampler(0, FilterType::Bilinear, AddressType::Clamp);
		m_Material->SetSampler(1, FilterType::Bilinear, AddressType::Wrap);
	}

	void Box::Render(DirectX::XMMATRIX& viewProj) const
	{
		m_VertexBuffer->Bind(0);
		m_IndexBuffer->Bind();
		m_Topology.Bind();

		m_TransformBuffer->SetViewProjection(viewProj);
		m_TransformBuffer->UpdateBuffer(m_TransformBuffer->GetMatrixBuffer());
		m_TransformBuffer->SetTransformBuffer();

		m_Material->BindMaterial();

		RendererContext::GetDeviceContext()->DrawIndexed(m_IndexBuffer->GetIndexCount(), 0, 0);
	}

	void Yassin::Box::UpdateLighting(const LightPositionBuffer& lPos, const LightPropertiesBuffer& lProps) const
	{
		m_Material->UpdateLightBuffers(lPos, lProps);
	}

	void Box::UpdateShadowMap(ID3D11ShaderResourceView* srv) const
	{
		m_Material->SetShadowMap(TextureSlot::DepthMapTexture, srv);
	}

	void Box::UnbindSRV() const
	{
		ID3D11ShaderResourceView* nullSRV = { nullptr };
		RendererContext::GetDeviceContext()->PSSetShaderResources(1, 1, &nullSRV);
	}
}