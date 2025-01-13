#pragma once
#include "IImGuiAPI.h"

namespace crab
{
	class ImGuiRenderer
	{
	public:
		static void Init(const ApplicationSetting& in_setting);
		static void Shutdown();

		static void BeginRender();
		static void EndRender();

	private:
		inline static Scope<IImGuiAPI> m_api = nullptr;
	};
}