#pragma once

namespace crab
{
	//===================================================
	//			    Immutable (Default)
	//===================================================

	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = default;
		virtual void	Bind() = 0;

		virtual void	SetStride(uint32 in_stride) = 0;
		virtual void	SetOffset(uint32 in_offset) = 0;

		virtual uint32	GetStride() const = 0;
		virtual uint32	GetOffset() const = 0;

		template<typename VertexType>
		static Ref<IVertexBuffer>	Create(const std::vector<VertexType>& in_vertices);

	private:
		friend class IDynamicVertexBuffer;
		virtual void				_create_(const void* in_vertices, uint32 in_vertexSize, uint32 in_verticesCount) = 0;
		static Ref<IVertexBuffer>	_create_concrete_vertex_buffer_(eRenderAPI in_api);
	};

	//===================================================
	//			          Dynamic
	//===================================================

	class IDynamicVertexBuffer : public IVertexBuffer
	{
	public:
		virtual uint32	GetBufferCapacity() const = 0;
		virtual uint32	GetBufferSize() const = 0;

		virtual void	PushVertices(const void* in_vertices, uint32 in_vertexSize, uint32 in_vertexCount) = 0;
		virtual void	Reallocate(uint32 in_nextCapacity) = 0;
		virtual void	ClearBuffer() = 0;

		template<typename VertexType>
		static Ref<IDynamicVertexBuffer>	Create(uint32 in_bufferSize) { return Create<VertexType>({}, in_bufferSize); }

		template<typename VertexType>
		static Ref<IDynamicVertexBuffer>	Create(const std::vector<VertexType>& in_vertices, uint32 in_bufferSize);

	private:
		static Ref<IDynamicVertexBuffer>	_create_concrete_dynamic_vertex_buffer_(eRenderAPI in_api);
	};

	//===================================================
	//                      Inline
	//===================================================

	template <typename VertexType>
	Ref<IDynamicVertexBuffer> IDynamicVertexBuffer::Create(const std::vector<VertexType>& in_vertices, uint32 in_bufferSize)
	{
		Ref<IDynamicVertexBuffer> buffer = _create_concrete_dynamic_vertex_buffer_(Renderer::GetRenderAPIType());
		buffer->_create_(in_vertices.data(), sizeof(VertexType), in_bufferSize);
		return buffer;
	}
}
