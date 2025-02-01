#pragma once
#include "Renderer/Primitives/CBufferStructTypes.h"
#include "Renderer/Primitives/ConstantBuffer.h"

namespace Yassin
{
	class TransformBuffer : public ConstantBuffer<MatrixBuffer>
	{
	public:
		inline void SetWorld(DirectX::XMMATRIX& world) { m_MatrixBuffer.world = world; }
		inline void SetView(DirectX::XMMATRIX& view) { m_MatrixBuffer.view = view; }
		inline void SetProjection(DirectX::XMMATRIX& proj) { m_MatrixBuffer.proj = proj; }
		inline void SetViewProjection(DirectX::XMMATRIX& viewProj) { m_MatrixBuffer.viewProj = viewProj; }

		inline DirectX::XMMATRIX& GetWorld() { return m_MatrixBuffer.world; }
		inline const DirectX::XMMATRIX& GetViewProjection() const { return m_MatrixBuffer.viewProj; }

		inline MatrixBuffer& GetMatrixBuffer() { return m_MatrixBuffer; }

		inline void SetTransformBuffer()
		{
			RendererContext::GetDeviceContext()->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
		}

	private:
		MatrixBuffer m_MatrixBuffer;
	};

	class LightPosBuffer : public VertexConstantBuffer<LightPositionBuffer> {};

	class LightPropsBuffer : public PixelConstantBuffer<LightPropertiesBuffer> {};

	class TransparencyBuffer : public PixelConstantBuffer<BlendBuffer> {};

	class FlatColorBuffer : public PixelConstantBuffer<FlatColorBufferType> {};

	class CameraBuffer : public VertexConstantBuffer<CameraPositionType> {};

	class LightDirectionBuffer : public PixelConstantBuffer<LightDirectionType> {};

	class PointLightBatchBuffer : public PixelConstantBuffer<PointLightBatch> {};

	class CameraDataBuffer : public ComputeConstantBuffer<CameraData>
	{
	public:
		void SetInvProj(const DirectX::XMMATRIX& invProj) { m_Data.invProj = invProj; }
		void Update() { UpdateBuffer(m_Data); }

	private:
		CameraData m_Data;
	};

	class WorkGroupBuffer : public ComputeConstantBuffer<WorkGroupData>
	{
	public:
		void SetData(const WorkGroupData& data) { m_Data = data; }
		void Update() { UpdateBuffer(m_Data); }

	private:
		WorkGroupData m_Data;
	};

	class ClusterBatchBuffer : public ComputeConstantBuffer<ClusterBatch>
	{
	public:
		void SetBatch(Cluster* data) { m_Batch.clusters = data; }
		void Update()
		{
			UpdateBuffer(m_Batch);
		}

	private:
		ClusterBatch m_Batch;
	};

	class LightCullingBatchBuffer : public ComputeConstantBuffer<LightCullingBatch>
	{
	public:
		void SetBatch(LightCullingData* data) { m_Batch.lights = data; }
		void Update()
		{
			UpdateBuffer(m_Batch);
		}

	private:
		LightCullingBatch m_Batch;
	};

	class SSAOPropertiesBuffer : public PixelConstantBuffer<SSAOProperties>
	{
	public:
		SSAOProperties& GetProperties() { return m_Props; };
		void SetProperties(const SSAOProperties& props) { m_Props = props; }

		void Update()
		{
			UpdateBuffer(m_Props);
		}

	private:
		SSAOProperties m_Props;
	};

	class ShadowAtlasBuffer : public ComputeConstantBuffer<ShadowAtlasType>
	{
	public:
		void SetOffsets(const ShadowAtlasType& offsets)
		{
			m_Offsets = offsets;
		}

		void Update()
		{
			UpdateBuffer(m_Offsets);
		}

	private:
		ShadowAtlasType m_Offsets;
	};

	class ScreenBuffer : public PixelConstantBuffer<ScreenPropertiesBuffer>
	{
	public:
		void SetHorizontalBlur() { m_ScreenProps.blurType = 0.0f; }
		void SetVerticalBlur() { m_ScreenProps.blurType = 1.0f; }
		void SetWidth(float width) { m_ScreenProps.screenWidth = width; }
		void SetHeight(float height) { m_ScreenProps.screenHeight = height; }

		void Update()
		{
			UpdateBuffer(m_ScreenProps);
		}
	private:
		ScreenPropertiesBuffer m_ScreenProps;
	};
}