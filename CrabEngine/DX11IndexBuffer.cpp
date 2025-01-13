#include "CrabEnginePch.h"
#include "DX11IndexBuffer.h"

namespace crab
{
	void DX11IndexBuffer::Bind()
	{
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();
		context->IASetIndexBuffer(m_bufData.buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11IndexBuffer::_create_(const std::vector<uint32>& in_indices, uint32 in_bufferSize)
	{
		auto device = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();

		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = static_cast<UINT>(sizeof(UINT) * in_indices.size());
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = in_indices.data();

		DX_ASSERT(device->CreateBuffer(&desc, &initData, m_bufData.buffer.GetAddressOf()), "index buffer create fail.");
		m_bufData.indexCount = static_cast<uint32>(in_indices.size());
	}

	void DX11DynamicIndexBuffer::_create_(const std::vector<uint32>& in_indices, uint32 in_bufferSize)
	{
		auto device = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();

		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = static_cast<UINT>(sizeof(UINT) * in_bufferSize);
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA initData = {};
		if (!in_indices.empty())
		{
			initData.pSysMem = in_indices.data();
		}

		DX_ASSERT(device->CreateBuffer(&desc, in_indices.empty() ? nullptr : &initData, m_bufData.buffer.GetAddressOf()), "index buffer create fail.");
		m_bufData.indexCount = static_cast<uint32>(in_indices.size());
		m_bufCap = in_bufferSize;
	}
}
