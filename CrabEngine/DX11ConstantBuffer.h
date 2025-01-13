#pragma once
#include "IConstantBuffer.h"

namespace crab
{
	class DX11ConstantBuffer : public IConstantBuffer
	{
	public:
		void Bind(eShaderFlags in_flag, eConstantBufferSlot in_slots) override
		{
			auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();
			if (in_flag & eShaderFlags_VertexShader) { context->VSSetConstantBuffers((uint32)in_slots, 1, m_buffer.GetAddressOf()); }
			if (in_flag & eShaderFlags_PixelShader) { context->PSSetConstantBuffers((uint32)in_slots, 1, m_buffer.GetAddressOf()); }
		}

	private:
		void _create_(uint32 in_vertexDataSize) override;
		void _set_data_(const void* in_data, uint32 in_dataSize) override;

		::ComPtr<ID3D11Buffer> m_buffer;
	};
}
