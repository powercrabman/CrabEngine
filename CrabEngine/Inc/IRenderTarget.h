#pragma once
#include <variant>

namespace crab
{
	struct RenderTargetProp
	{
		uint32	width;
		uint32	height;
		uint32	minDepth;
		uint32	maxDepth;

		Vec4	clearColor;
	};

	class IRenderTarget
	{
	public:
		IRenderTarget() = default;
		virtual ~IRenderTarget() = default;

		virtual void					Clear() = 0;
		virtual void					SetClearColor(const Vec4& in_color) = 0;

		virtual void*					GetNativeRenderTarget() = 0;
		virtual void*					GetViewport() = 0;
		virtual void*					GetTexture() const = 0;
		virtual std::pair<float, float>	GetSize() const = 0;
		virtual float					GetAspect() const = 0;

		static Ref<IRenderTarget>		Create(const RenderTargetProp& in_prop);

	private:
		virtual void					_create_(const RenderTargetProp& in_prop) = 0;
	};
}
