#pragma once
#include "IIndexBuffer.h"

namespace crab
{
	class DX11IndexBuffer : public IIndexBuffer
	{
	public:
		DX11IndexBuffer() = default;
		~DX11IndexBuffer() = default;

		void	Bind() override;
		uint32	GetIndexCount() const override { return m_indexCount; }

	private:
		void _create_(const std::initializer_list<uint32>& in_indices) override;

		::ComPtr<ID3D11Buffer>	m_buffer     = 0;
		uint32					m_indexCount = 0;
	};
}
