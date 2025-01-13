#include "CrabEnginePch.h"
#include "DX11VertexBuffer.h"

namespace crab
{
	//===================================================
	//			         Immutable
	//===================================================

	void DX11VertexBuffer::Bind()
	{
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();
		context->IASetVertexBuffers(0, 1, m_bufData.buffer.GetAddressOf(), &m_bufData.stride, &m_bufData.offset);
	}

	void DX11VertexBuffer::_create_(const void* in_vertices, uint32 in_vertexSize, uint32 in_verticesCount)
	{
		auto device = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();

		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = static_cast<UINT>(in_vertexSize * in_verticesCount);
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = in_vertices;
		ASSERT(in_vertices, "immutable buffer have to initialize with vertices");

		DX_ASSERT(device->CreateBuffer(&desc, &initData, m_bufData.buffer.GetAddressOf()), "Cannot create Vertex Buffer");

		// Set Stride & Offset
		m_bufData.stride = in_vertexSize;
		m_bufData.offset = 0;
	}

	//===================================================
	//			          Dynamic
	//===================================================

	void DX11DynamicVertexBuffer::Bind()
	{
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();
		context->IASetVertexBuffers(0, 1, m_bufData.buffer.GetAddressOf(), &m_bufData.stride, &m_bufData.offset);
	}

	void DX11DynamicVertexBuffer::PushVertices(const void* in_vertices, uint32 in_vertexSize, uint32 in_vertexCount)
	{
		assert(m_bufData.stride == in_vertexSize);

		if (m_bufSize + in_vertexCount > m_bufCap)
		{
			Reallocate(m_bufSize + in_vertexCount);
		}

		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();

		D3D11_MAPPED_SUBRESOURCE mapRes = {};
		DX_ASSERT(context->Map(m_bufData.buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes), "dynamic vertex buffer map fail.");

		memcpy(static_cast<char*>(mapRes.pData) + (m_bufSize * m_bufData.stride), in_vertices, in_vertexCount * m_bufData.stride);
		context->Unmap(m_bufData.buffer.Get(), 0);

		m_bufSize += in_vertexCount;
	}

	void DX11DynamicVertexBuffer::Reallocate(uint32 in_newCapacity)
	{
		if (in_newCapacity <= m_bufCap) return;
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();

		std::vector<char> tempData(m_bufSize * m_bufData.stride);
		{
			D3D11_MAPPED_SUBRESOURCE mapRes = {};
			DX_ASSERT(context->Map(m_bufData.buffer.Get(), 0, D3D11_MAP_READ, 0, &mapRes), "dynamic vertex buffer map fail.");

			memcpy(tempData.data(), mapRes.pData, m_bufSize * m_bufData.stride);
			context->Unmap(m_bufData.buffer.Get(), 0);
		}

		_create_(tempData.data(), m_bufData.stride, in_newCapacity);

		if (!tempData.empty())
		{
			D3D11_MAPPED_SUBRESOURCE mapRes = {};
			DX_ASSERT(context->Map(m_bufData.buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes), "dynamic vertex buffer map fail.");

			memcpy(mapRes.pData, tempData.data(), m_bufSize * m_bufData.stride);
			context->Unmap(m_bufData.buffer.Get(), 0);
		}

		m_bufCap = in_newCapacity;
	}

	void DX11DynamicVertexBuffer::_create_(const void* in_vertices, uint32 in_vertexSize, uint32 in_verticesCount)
	{
		auto device = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();

		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = static_cast<UINT>(in_vertexSize * in_verticesCount);
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA initData = {};
		if (in_vertices)
		{
			initData.pSysMem = in_vertices;
		}

		DX_ASSERT(device->CreateBuffer(&desc, in_vertices ? &initData : nullptr, m_bufData.buffer.GetAddressOf()), "Cannot create Vertex Buffer");

		m_bufData.stride = in_vertexSize;
		m_bufData.offset = 0;
		m_bufCap = in_verticesCount;
	}


	//void DX11VertexBuffer::UpdateVertexBuffer(const void* in_newData)
	//{
	//	auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();
	//	context->UpdateSubresource(m_buffer.Get(), 0, nullptr, in_newData, 0, 0);
	//}
}
