#pragma once

namespace crab
{
	class IConstantBuffer
	{
	public:
		IConstantBuffer() = default;
		virtual ~IConstantBuffer() = default;

		virtual void Bind(eShaderFlags in_flag, eConstantBufferSlot in_slots) = 0;

		template <typename DataType>
		void SetData(const DataType& in_data)
		{
			_set_data_(&in_data, sizeof(DataType));
		}

		template <typename DataType>
		static Ref<IConstantBuffer> Create()
		{
			Ref<IConstantBuffer> buffer = _create_concrete_constant_buffer_(Renderer::GetRenderAPIType());
			buffer->_create_(sizeof(DataType));
			return buffer;
		}

	private:
		static Ref<IConstantBuffer> _create_concrete_constant_buffer_(eRenderAPI in_api);
		virtual void _create_(uint32 in_vertexDataSize) = 0;
		virtual void _set_data_(const void* in_data, uint32 in_dataSize) = 0;
	};
}
