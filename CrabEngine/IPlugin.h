#pragma once

namespace crab
{
	// 해당 클래스를 상속하여 추가적인 플러그인 추가가능

	class Scene;

	class IPlugin
	{
	public:
				 IPlugin() = default;
		virtual ~IPlugin() = default;

		virtual void OnAttach()                      {}
		virtual void OnDetach()                      {}
		virtual void OnEnterScene()                  {}
		virtual void OnExitScene()                   {}
		virtual void OnUpdateScene(TimeStamp& in_ts) {}
		virtual void OnRenderScene(TimeStamp& in_ts) {}
		virtual void OnImGuiRender(TimeStamp& in_ts) {}

		virtual TypeID	GetTypeID() = 0;
	};
}
