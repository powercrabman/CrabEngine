#pragma once

namespace crab
{
	class Scene;

	class Entity
	{
	public:
		Entity(Scene* in_scene, entt::entity in_entity);
		Entity(entt::registry& in_registry, entt::entity in_entity);

		operator entt::entity() const { return m_entity; }

		template<typename Ty, typename ...Args>
		auto TryCreateComponent(Args... args)
		{
			return m_registry->emplace_or_replace<Ty>(m_entity, std::forward<Args>(args)...);
		}

		template<typename Ty>
		auto TryRemoveComponent()
		{
			m_registry->erase<Ty>(m_entity);
		}

		template<typename Ty>
		Ty* TryGetComponent()
		{
			return m_registry->try_get<Ty>(m_entity);
		}

		static Entity s_nullEntity;

	private:
		entt::registry* m_registry = nullptr;
		entt::entity	m_entity   = entt::null;
	};
}
