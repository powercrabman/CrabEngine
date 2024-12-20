#include "CrabEnginePch.h"
#include "DX11VertexBuffer.h"

namespace crab
{
	void DX11VertexBuffer::Bind()
	{
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();
		context->IASetVertexBuffers(0, 1, m_buffer.GetAddressOf(), &m_stride, &m_offset);
	}

	void DX11VertexBuffer::_create_(const void* in_vertices, uint32 in_verticesSize, uint32 in_verticesCount)
	{
		auto device = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();

		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = static_cast<UINT>(in_verticesSize * in_verticesCount);
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = in_vertices;

		HR hr = device->CreateBuffer(&desc, &initData, m_buffer.GetAddressOf());

		DX_ASSERT(hr, "Cannot create Vertex Buffer");

		// Set Stride & Offset
		m_stride = in_verticesSize;
		m_offset = 0;
	}

}
