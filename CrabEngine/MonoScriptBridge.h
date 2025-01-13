#pragma once
#include "MonoScriptEngine.h"

namespace crab
{
	class MonoScriptBridge : public Singleton<MonoScriptBridge>
	{
		friend class Singleton<MonoScriptBridge>;
		struct EntityComponentFunc;

		using EntityComponentMap = std::unordered_map<MonoType*, EntityComponentFunc>;

	public:
		void Init();

		template <typename Ty>
		void RegisterComponent();

		const EntityComponentMap& GetEntityComponentMap() const { return m_entityComponentFunc; }

		template <typename... Ty>
		void RegisterComponentGroup(ComponentGroup<Ty...>)
		{
			(RegisterComponent<Ty>(), ...);
		}

	private:
		struct EntityComponentFunc
		{
			std::function<MonoInstance(Entity)> tryCreateComponent;
			std::function<MonoInstance(Entity)> createComponent;
			std::function<MonoInstance(Entity)> replaceComponent;
			std::function<bool(Entity)> hasComponent;
			std::function<MonoInstance(Entity)> tryFindComponent;
			std::function<MonoInstance(Entity)> findComponent;
			std::function<void(Entity)> removeComponent;
		};
		EntityComponentMap m_entityComponentFunc;
	};

	FORCEINLINE NODISCARD MonoScriptBridge& GetMonoScriptBridge() { return MonoScriptBridge::Get(); }

	template <typename Ty>
	void MonoScriptBridge::RegisterComponent()
	{
		std::string name = fmt::format("crab.{}", Ty::s_staticName);
		MonoType* type = mono_reflection_type_from_name(
			name.data(),
			MonoScriptEngine::GetCoreAssemblyData().image
		);

		EntityComponentFunc func = {};
		func.tryCreateComponent = [](Entity e)
		{
			Ty& cmp = e.TryCreateComponent<Ty>();
			MonoInstance inst = MonoScriptEngine::GetCoreAssemblyData().CreateMonoInstance("crab", Ty::s_staticName);
			return inst;
		};
		func.createComponent = [](Entity e)
		{
			Ty& cmp = e.CreateComponent<Ty>();
			MonoInstance inst = MonoScriptEngine::GetCoreAssemblyData().CreateMonoInstance("crab", Ty::s_staticName);
			return inst;
		};
		func.replaceComponent = [](Entity e)
		{
			Ty& cmp = e.ReplaceComponent<Ty>();
			MonoInstance inst = MonoScriptEngine::GetCoreAssemblyData().CreateMonoInstance("crab", Ty::s_staticName);
			return inst;
		};
		func.hasComponent = [](Entity e) { return e.HasComponent<Ty>(); };
		func.tryFindComponent = [](Entity e)
		{
			Ty* cmp = e.TryFindComponent<Ty>();
			if (cmp)
			{
				MonoInstance inst = MonoScriptEngine::GetCoreAssemblyData().CreateMonoInstance("crab", Ty::s_staticName);
				return inst;
			}
			return MonoInstance::s_null;
		};
		func.findComponent = [](Entity e)
		{
			Ty& cmp = e.FindComponent<Ty>();
			MonoInstance inst = MonoScriptEngine::GetCoreAssemblyData().CreateMonoInstance("crab", Ty::s_staticName);
			return inst;
		};
		func.removeComponent = [](Entity e) { e.RemoveComponent<Ty>(); };

		m_entityComponentFunc.emplace(type, func);
	}

	//===================================================
	//				 Bridge Functions
	//===================================================


#pragma region Log

	// Log
	static void Log_Trace(MonoString* in_msg);
	static void Log_Debug(MonoString* in_msg);
	static void Log_Error(MonoString* in_msg);
	static void Log_Warn(MonoString* in_msg);
	static void Log_Info(MonoString* in_msg);

#pragma endregion

#pragma region Input

	static bool Input_IsKeyPress(eKey in_key);
	static bool Input_IsKeyRelease(eKey in_key);
	static bool Input_IsKeyDown(eKey in_key);
	static bool Input_IsKeyUp(eKey in_key);

#pragma endregion

#pragma region Entity

	// Entity Create
	static MonoObject* Entity_TryCreateComponent(uint32 in_entityID, MonoReflectionType* in_componentType);
	static MonoObject* Entity_CreateComponent(uint32 in_entityID, MonoReflectionType* in_componentType);
	static MonoObject* Entity_ReplaceComponent(uint32 in_entityID, MonoReflectionType* in_componentType);

	// Entity Find
	static bool			Entity_HasComponent(uint32 in_entityID, MonoReflectionType* in_componentType);
	static MonoObject* Entity_TryFindComponent(uint32 in_entityID, MonoReflectionType* in_componentType);
	static MonoObject* Entity_FindComponent(uint32 in_entityID, MonoReflectionType* in_componentType);

	// Entity Remove
	static void Entity_RemoveComponent(uint32 in_entityID, MonoReflectionType* in_componentType);
	static bool IsValid(uint32 in_entityID);

#pragma endregion

#pragma region Transform

	static void Transform_SetPosition(uint32 in_entityID, const Vec3& in_position);
	static void Transform_GetPosition(uint32 in_entityID, Vec3& out_position);
	static void Transform_SetRotation(uint32 in_entityID, const Vec3& in_rotation);
	static void Transform_GetRotation(uint32 in_entityID, Vec3& out_position);
	static void Transform_SetScale(uint32 in_entityID, const Vec3& in_scaling);
	static void Transform_GetScale(uint32 in_entityID, Vec3& out_scaling);

#pragma endregion
}