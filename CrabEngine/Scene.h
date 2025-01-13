#pragma once
#include "Entity.h"

namespace crab
{

#define REGISTER_SCENE(scene)\
	const char* ToString() const override {return #scene; };\
	inline static bool _regist_scene_ = []() { crab::SceneManager::Get().EmplaceScene<scene>(); return true; }();\
	inline static const char* s_staticNamt = #scene

	class Scene
	{
	public:
				 Scene() = default;
		virtual ~Scene() = default;

		// Core
		virtual void	OnSetupSceneSuper()				final;
		virtual void	OnEnterSceneSuper()				final;
		virtual void	OnExitSceneSuper()				final;

		virtual void	OnUpdateSuper(TimeStamp& in_ts)	final;
		virtual void	OnRenderSuper(TimeStamp& in_ts)	final;
		virtual void	OnImGuiRender(TimeStamp& in_ts)	= 0;

		virtual void	OnEventSuper(IEvent& in_event) final;

		void			ClearScene() { m_registry.clear(); }

		virtual std::filesystem::path	GetSceneDataPath() const;
		virtual const char*				ToString() const	= 0;

		// About ECS
		Entity			CreateEntity();
		Entity			CreateEntity(uint32 in_id);
		void			DeleteEntity(Entity in_entity);
		Entity			DuplicateEntity(Entity in_srcEntity);
		Entity			TryFindByID(uint32 in_id);;

		entt::registry& GetRegistry() { return m_registry; }

		template<typename ...Types>
		auto			GetView() { return m_registry.view<Types...>(); }

	private:
		virtual void	OnSetupScene()				= 0;
		virtual void	OnEnterScene()				= 0;
		virtual void	OnExitScene()				= 0;

		virtual void	OnUpdate(TimeStamp& in_ts)	= 0;
		virtual void	OnRender(TimeStamp& in_ts)	= 0;
		virtual void	OnEvent(IEvent& in_event)   = 0;

		entt::registry	m_registry;

		bool			m_isSetuped = false;
	};
}
