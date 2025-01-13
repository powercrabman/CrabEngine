#pragma once

namespace crab
{
	class NativeScript
	{
	public:
		NativeScript(Entity in_ownerEntity) : m_owner(in_ownerEntity) {}
		virtual ~NativeScript() = default;

		virtual void OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnUpdate(TimeStamp& in_ts) = 0;

		template<typename Ty, typename ...Args>
		auto& TryCreateComponent(Args&&... args)
		{
			return m_owner.TryCreateComponent<Ty>(args...);
		}

		template<typename Ty, typename ...Args>
		auto& CreateComponent(Args&&... args)
		{
			return m_owner.CreateComponent<Ty>(args...);
		}

		template<typename Ty, typename ...Args>
		auto& ReplaceComponent(Args&&... args)
		{
			return m_owner.ReplaceComponent<Ty>(args...);
		}

		// Remove Component

		template<typename Ty>
		void RemoveComponent() const
		{
			m_owner.RemoveComponent<Ty>();
		}

		template<typename Ty>
		Ty* TryFindComponent()
		{
			return m_owner.TryFindComponent<Ty>();
		}

		template<typename Ty>
		Ty& FindComponent()
		{
			return m_owner.FindComponent<Ty>();
		}

		template<typename Ty>
		bool HasComponent() const
		{
			return m_owner.HasComponent<Ty>();
		}

		// Etc..
		bool IsValid() const
		{
			return m_owner.IsValid();
		}

		Entity GetOwner() const { return m_owner; }

	private:
		Entity m_owner = Entity::s_null;
	};
}

