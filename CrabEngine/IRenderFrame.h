#pragma once
namespace crab
{
	class IDepthStencil;
	class IUnorderedAccessView;

	struct RenderFrameProp
	{
		std::vector<Ref<IRenderTarget>>			renderTargets;			// essential
		std::vector<Ref<IUnorderedAccessView>>	unorderedAccessViews;	// option
		Ref<IDepthStencil>						depthStencil;			// option
	};

	class IRenderFrame
	{
	public:
				 IRenderFrame() = default;
		virtual ~IRenderFrame() = default;

		virtual uint32				GetRenderTargetCount() = 0;
		virtual void				Clear(bool in_clearDepth, bool in_clearStencil) = 0;
		virtual void				Bind() = 0;
		virtual void				UnBind() = 0;

		virtual Ref<IRenderTarget>			GetRenderTarget(uint32 in_slot) = 0;
		virtual Ref<IUnorderedAccessView>	GetUnorderedResourceView(uint32 in_slot) = 0;

		static Ref<IRenderFrame> Create(const RenderFrameProp& in_prop);

	private:
		virtual void _create_(const RenderFrameProp& in_prop) = 0;
	};
}

