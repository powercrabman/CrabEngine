#pragma once

namespace crab
{
	class IVertexBuffer
	{
	public:
		virtual void	Bind() = 0;

		virtual void	SetStride(uint32 in_stride) = 0;
		virtual void	SetOffset(uint32 in_offset) = 0;

		virtual uint32	GetStride() const = 0;
		virtual uint32	GetOffset() const = 0;

		template<typename VertexType>
		static Ref<IVertexBuffer>	Create(const std::initializer_list<VertexType>& in_vertices);

	private:
		virtual void				_create_(const void* in_vertices, uint32 in_verticesSize, uint32 in_verticesCount) = 0;
		static Ref<IVertexBuffer>	_create_concrete_vertex_buffer_(eRenderAPI in_api);
	};

	//===================================================
	//                      Inline
	//===================================================

	template<typename VertexType>
	inline Ref<IVertexBuffer> IVertexBuffer::Create(const std::initializer_list<VertexType>& in_vertices)
	{
		Ref<IVertexBuffer> buffer = _create_concrete_vertex_buffer_(Renderer::GetRednerAPIType());
		buffer->_create_(in_vertices.begin(), sizeof(VertexType), in_vertices.size());
		return buffer;
	}
}