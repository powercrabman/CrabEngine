#pragma once

namespace crab
{
	class IDepthStencil
	{
	public:
				 IDepthStencil() = default;
		virtual ~IDepthStencil() = default;

		virtual void*	GetNativeDepthStencil() = 0;
		virtual void	Clear(bool in_clearDepth, bool in_clearStencil) = 0;

		static Ref<IDepthStencil>	Create(uint32 in_width, uint32 in_height);

	private:
		virtual void create_depthstencil(uint32 in_width, uint32 in_height) = 0;
	};
}


