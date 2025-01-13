using System;
using System.Diagnostics.Eventing.Reader;

namespace crab
{
	public abstract class Entity
	{
		public Entity(UInt32 in_entityID) { ID = in_entityID; }
		public Entity() { ID = 0; }

		public bool HasComponent<T>() where T : Component
		{
			return InternalCalls.Entity_HasComponent(ID, typeof(T));
		}
		public T TryFindComponent<T>() where T : Component
		{
			T component = (T)InternalCalls.Entity_TryFindComponent(ID, typeof(T));
			component.entity = this;
			return component;
		}
		public T FindComponent<T>() where T : Component
		{
			T component = (T)InternalCalls.Entity_FindComponent(ID, typeof(T));
			component.entity = this;
			return component;
		}
		public T TryCreateComponent<T>() where T : Component
		{
			T component = (T)InternalCalls.Entity_TryCreateComponent(ID, typeof(T));
			component.entity = this;
			return component;
		}
		public T CreateComponent<T>() where T : Component
		{
			T component = (T)InternalCalls.Entity_CreateComponent(ID, typeof(T));
			component.entity = this;
			return component;
		}
		public T ReplaceComponent<T>() where T : Component
		{
			T component = (T)InternalCalls.Entity_ReplaceComponent(ID, typeof(T));
			component.entity = this;
			return component;
		}
		public void RemoveComponent<T>() where T : Component
		{
			InternalCalls.Entity_RemoveComponent(ID, typeof(T));
		}
		public bool IsValid()
		{
			return InternalCalls.IsValid(ID);
		}

		public abstract void OnCreate();
		public abstract void OnUpdate(float in_detalTime);

		public readonly UInt32 ID;
	}
}
