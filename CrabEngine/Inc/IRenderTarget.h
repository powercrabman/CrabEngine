#pragma once
#include "ITexture.h"

namespace crab
{
	struct RenderTargetProp
	{
		float			positionX;
		float			positionY;
		float			width;
		float			height;
		float			depthMin;
		float			depthMax;
	};

	class IRenderTarget
	{
	public:
		IRenderTarget() = default;
		virtual ~IRenderTarget() = default;

		virtual void					SetViewport(float in_positionX, float in_positionY, float in_width, float in_height, float in_depthMin, float in_depthMax) = 0;
		virtual void*					GetTexture() const = 0;

		virtual void					Clear(const Vec4& in_clearColor, bool in_clearDepth, bool in_clearStencil) = 0;
		virtual std::pair<float, float>	GetSize() const = 0;
		virtual float					GetAspect() const = 0;

		static Ref<IRenderTarget>		Create(const RenderTargetProp& in_prop);

	private:
		virtual void	_create_(const RenderTargetProp& in_prop) = 0;
	};

}
