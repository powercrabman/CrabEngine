#pragma once
#include "IIndexBuffer.h"

namespace crab
{
	struct DX11IndexBufferData
	{
		::ComPtr<ID3D11Buffer>	buffer = nullptr;
		uint32					indexCount = 0;
	};

	//===================================================
	//			     Immutable (Default)
	//===================================================

	class DX11IndexBuffer : public IIndexBuffer
	{
	public:
		DX11IndexBuffer() = default;
		~DX11IndexBuffer() = default;

		void	Bind() override;
		uint32	GetIndexCount() const override { return m_bufData.indexCount; }

	private:
		void _create_(const std::vector<uint32>& in_indices, uint32 in_bufferSize) override;

		DX11IndexBufferData m_bufData;
	};

	//===================================================
	//			          Dynamic
	//===================================================

	class DX11DynamicIndexBuffer : public IDynamicIndexBuffer
	{
	public:
		void	Bind() override;

		void	PushIndices(const std::vector<uint32>& in_indices) override;
		void	Reallocate(uint32 in_nextCapacity) override;
		void	ClearBuffer() override;

		uint32	GetIndexCount() const override { return m_bufData.indexCount; }
		uint32	GetBufferCapacity() const override { return m_bufCap; }

	private:
		void _create_(const std::vector<uint32>& in_indices, uint32 in_bufferSize) override;

		DX11IndexBufferData		m_bufData;
		uint32					m_bufCap = 0;
	};

	inline void DX11DynamicIndexBuffer::Bind()
	{
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();
		context->IASetIndexBuffer(m_bufData.buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	inline void DX11DynamicIndexBuffer::PushIndices(const std::vector<uint32>& in_indices)
	{
		if (m_bufData.indexCount + in_indices.size() > m_bufCap)
		{
			Reallocate(m_bufCap + static_cast<uint32>(in_indices.size()));
		}

		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();
		D3D11_MAPPED_SUBRESOURCE mapRes = {};
		HRESULT hr = context->Map(m_bufData.buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
		DX_ASSERT(SUCCEEDED(hr), "Failed to map dynamic index buffer.");

		memcpy(static_cast<char*>(mapRes.pData) + (m_bufData.indexCount * sizeof(uint32)), in_indices.data(), in_indices.size() * sizeof(uint32));
		context->Unmap(m_bufData.buffer.Get(), 0);

		m_bufData.indexCount += static_cast<uint32>(in_indices.size());
	}

	inline void DX11DynamicIndexBuffer::Reallocate(uint32 in_nextCapacity)
	{
		if (in_nextCapacity <= m_bufCap) return;
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();

		std::vector<uint32> tempData(m_bufData.indexCount);
		if (m_bufData.indexCount > 0)
		{
			D3D11_MAPPED_SUBRESOURCE mapRes = {};
			DX_ASSERT(context->Map(m_bufData.buffer.Get(), 0, D3D11_MAP_READ, 0, &mapRes), "dynamic buffer map error.");

			memcpy(tempData.data(), mapRes.pData, m_bufData.indexCount * sizeof(uint32));
			context->Unmap(m_bufData.buffer.Get(), 0);
		}

		_create_(tempData, in_nextCapacity);
		m_bufCap = in_nextCapacity;
	}

	inline void DX11DynamicIndexBuffer::ClearBuffer()
	{
		m_bufData.indexCount = 0;
	}
}
