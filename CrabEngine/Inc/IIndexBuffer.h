#pragma once

namespace crab
{
	class IIndexBuffer
	{
	public:
		IIndexBuffer() = default;
		virtual ~IIndexBuffer() = default;

		virtual void	Bind() = 0;
		virtual uint32	GetIndexCount() const = 0;

		static Ref<IIndexBuffer> Create(const std::initializer_list<uint32>& in_indices);

	private:
		virtual void	_create_(const std::initializer_list<uint32>& in_indices) = 0;
	};
}
