#pragma once

namespace crab
{
	class IImGuiAPI
	{
	public:
		IImGuiAPI() = default;
		virtual ~IImGuiAPI() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void RenderBegin() = 0;
		virtual void RenderEnd() = 0;

	};
}
