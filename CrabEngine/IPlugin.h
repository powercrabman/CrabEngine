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

		virtual void	Init()                          = 0;
		virtual void	Shutdown()                      = 0;
		virtual void	OnEnterScene()                  = 0;
		virtual void	OnExitScene()                   = 0;
		virtual void	OnUpdateScene(TimeStamp& in_ts) = 0;
		virtual void	OnRenderScene(TimeStamp& in_ts) = 0;

		virtual TypeID	GetTypeID() = 0;
	};
}
