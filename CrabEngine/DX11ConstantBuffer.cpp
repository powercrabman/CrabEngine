#include "CrabEnginePch.h"
#include "DX11ConstantBuffer.h"

void crab::DX11ConstantBuffer::_create_(uint32 in_vertexDataSize)
{
	auto device              = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();
	D3D11_BUFFER_DESC desc   = {};
	desc.ByteWidth           = in_vertexDataSize;
	desc.Usage               = D3D11_USAGE_DYNAMIC;
	desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags           = 0;
	desc.StructureByteStride = 0;

	DX_ASSERT(device->CreateBuffer(&desc, nullptr, m_buffer.GetAddressOf()), "constant buffer fail.");
}

void crab::DX11ConstantBuffer::_set_data_(const void* in_data, uint32 in_dataSize)
{
	auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();

	D3D11_MAPPED_SUBRESOURCE res;
	ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));

	DX_ASSERT(context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &res), "Map fail.");
	std::memcpy(res.pData, in_data, in_dataSize);

	context->Unmap(m_buffer.Get(), 0);
}
