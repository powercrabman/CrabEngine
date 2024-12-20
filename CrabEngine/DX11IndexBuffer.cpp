#include "CrabEnginePch.h"
#include "DX11IndexBuffer.h"

namespace crab
{
	void DX11IndexBuffer::Bind()
	{
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();
		context->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11IndexBuffer::_create_(const std::initializer_list<uint32>& in_indices)
	{
		auto device = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();

		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = static_cast<UINT>(sizeof(UINT) * in_indices.size());
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = in_indices.begin();

		DX_ASSERT(device->CreateBuffer(&desc, &initData, m_buffer.GetAddressOf()), "index buffer create fail.");
		m_indexCount = static_cast<uint32>(in_indices.size());
	}
}
