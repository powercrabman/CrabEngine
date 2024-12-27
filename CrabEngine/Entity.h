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
		bool operator==(const Entity& in_other) const { return m_entity == in_other.m_entity && m_registry == in_other.m_registry; }
		bool operator!=(const Entity& in_other) const { return !(*this == in_other); }

		uint32		 GetID() const { return (uint32)m_entity; }
		entt::entity GetEntity() const { return m_entity; }

		template<typename Ty, typename ...Args>
		auto TryCreateComponent(Args... args)
		{
			return m_registry->get_or_emplace<Ty>(m_entity, std::forward<Args>(args)...);
		}

		template<typename Ty, typename ...Args>
		auto CreateComponent(Args... args)
		{
			return m_registry->emplace<Ty>(m_entity, std::forward<Args>(args)...);
		}

		template<typename Ty>
		void RemoveComponent()
		{
			m_registry->remove<Ty>(m_entity);
		}

		template<typename Ty>
		Ty* TryGetComponent()
		{
			return m_registry->try_get<Ty>(m_entity);
		}

		template<typename Ty>
		Ty& GetComponent()
		{
			return m_registry->get<Ty>(m_entity);
		}

		template<typename Ty>
		bool HasComponent()
		{
			return m_registry->all_of<Ty>(m_entity);
		}

		bool IsValid()
		{
			return *this != s_nullEntity 
				|| m_registry ? m_registry->valid(m_entity) : false;
		}

		static Entity s_nullEntity;

	private:
		entt::registry* m_registry = nullptr;
		entt::entity	m_entity   = entt::null;
	};
}
