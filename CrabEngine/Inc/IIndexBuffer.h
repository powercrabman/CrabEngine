#pragma once

namespace crab
{
	//===================================================
	//			    Immutable (Default)
	//===================================================

	class IIndexBuffer
	{
	public:
		IIndexBuffer() = default;
		virtual ~IIndexBuffer() = default;

		virtual void	Bind() = 0;
		virtual uint32	GetIndexCount() const = 0;

		static Ref<IIndexBuffer> Create(const std::vector<uint32>& in_indices);

	private:
		friend class IDynamicIndexBuffer;
		virtual void	_create_(const std::vector<uint32>& in_indices, uint32 in_bufferSize) = 0;
	};

	//===================================================
	//			          Dynamic
	//===================================================

	class IDynamicIndexBuffer : public IIndexBuffer
	{
	public:
		virtual uint32	GetBufferCapacity() const = 0;

		virtual void	PushIndices(const std::vector<uint32>& in_indices) = 0;
		virtual void	Reallocate(uint32 in_nextCapacity) = 0;
		virtual void	ClearBuffer() = 0;

		static Ref<IDynamicIndexBuffer> Create(const std::vector<uint32>& in_indices, uint32 in_bufferSize);
		static Ref<IDynamicIndexBuffer> Create(uint32 in_bufferSize) { return Create({}, in_bufferSize); }
	};
}
