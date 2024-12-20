#pragma once
#include "IVertexBuffer.h"

namespace crab
{
	class DX11VertexBuffer : public IVertexBuffer
	{
	public:
		void Bind();

		void SetStride(uint32 in_stride) { m_stride = in_stride; }
		void SetOffset(uint32 in_offset) { m_offset = in_offset; }

		uint32 GetStride() const { return m_stride; }
		uint32 GetOffset() const { return m_offset; }

	private:
		void _create_(const void* in_vertices, uint32 in_verticesSize, uint32 in_verticesCount) override;

		::ComPtr<ID3D11Buffer>	m_buffer = nullptr;
		uint32					m_stride = 0;
		uint32					m_offset = 0;
	};
}