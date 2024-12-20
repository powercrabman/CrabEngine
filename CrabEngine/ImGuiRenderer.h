#pragma once
#include "IImGuiAPI.h"

namespace crab
{
	class ImGuiRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void RenderBegin();
		static void RenderEnd();

		static void OnEvent(IEvent& in_event);

	private:
		inline static Scope<IImGuiAPI> m_api = nullptr;
	};
}