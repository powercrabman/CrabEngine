#pragma once
#include "IVertexBuffer.h"

namespace crab
{
	struct Dx11VertexBufferData
	{
		::ComPtr<ID3D11Buffer>	buffer = nullptr;
		uint32					stride = 0;
		uint32					offset = 0;
	};

	//===================================================
	//			         Immutable
	//===================================================

	class DX11VertexBuffer : public IVertexBuffer
	{
	public:
		void	Bind() override;

		void	SetStride(uint32 in_stride) override { m_bufData.stride = in_stride; }
		void	SetOffset(uint32 in_offset) override { m_bufData.offset = in_offset; }

		uint32	GetStride() const override { return m_bufData.stride; }
		uint32	GetOffset() const override { return m_bufData.offset; }

	private:
		void	_create_(const void* in_vertices, uint32 in_vertexSize, uint32 in_verticesCount) override;

		Dx11VertexBufferData m_bufData;
	};

	//===================================================
	//					   Dynamic
	//===================================================

	class DX11DynamicVertexBuffer : public IDynamicVertexBuffer
	{
	public:
		void	Bind() override;

		void	PushVertices(const void* in_vertices, uint32 in_vertexSize, uint32 in_vertexCount) override;
		void	Reallocate(uint32 in_newCapacity) override;
		void	ClearBuffer() override { m_bufSize = 0; }

		void	SetStride(uint32 in_stride) override { m_bufData.stride = in_stride; }
		void	SetOffset(uint32 in_offset) override { m_bufData.offset = in_offset; }

		uint32	GetStride() const override { return m_bufData.stride; }
		uint32	GetOffset() const override { return m_bufData.offset; }

		uint32	GetBufferCapacity() const override { return m_bufCap; };
		uint32	GetBufferSize() const override { return m_bufSize; }

	private:
		void _create_(const void* in_vertices, uint32 in_vertexSize, uint32 in_verticesCount) override;


		enum { REALLOCATE_WEIGHT = 5000 };

		Dx11VertexBufferData	m_bufData;
		uint32					m_bufCap = 0;
		uint32					m_bufSize = 0;
	};


}
