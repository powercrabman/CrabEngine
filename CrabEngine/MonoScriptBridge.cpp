#include "CrabEnginePch.h"
#include "MonoScriptBridge.h"

#define REGISTER_MONO_METHOD(MethodName) mono_add_internal_call("crab.InternalCalls::" #MethodName, (const void*)MethodName);

namespace crab
{
	namespace
	{
		std::string ToString(MonoString* in_string)
		{
			char* str = mono_string_to_utf8(in_string);
			std::string retVal(str);
			mono_free(str);
			return retVal;
		}
	}

	void MonoScriptBridge::Init()
	{
		REGISTER_MONO_METHOD(Log_Trace);
		REGISTER_MONO_METHOD(Log_Debug);
		REGISTER_MONO_METHOD(Log_Error);
		REGISTER_MONO_METHOD(Log_Warn);
		REGISTER_MONO_METHOD(Log_Info);

		REGISTER_MONO_METHOD(Entity_TryCreateComponent);
		REGISTER_MONO_METHOD(Entity_CreateComponent);
		REGISTER_MONO_METHOD(Entity_ReplaceComponent);

		REGISTER_MONO_METHOD(Entity_HasComponent);
		REGISTER_MONO_METHOD(Entity_TryFindComponent);
		REGISTER_MONO_METHOD(Entity_FindComponent);

		REGISTER_MONO_METHOD(Entity_RemoveComponent);
		REGISTER_MONO_METHOD(IsValid);

		REGISTER_MONO_METHOD(Input_IsKeyPress);
		REGISTER_MONO_METHOD(Input_IsKeyRelease);
		REGISTER_MONO_METHOD(Input_IsKeyDown);
		REGISTER_MONO_METHOD(Input_IsKeyUp);

		REGISTER_MONO_METHOD(Transform_SetPosition);
		REGISTER_MONO_METHOD(Transform_GetPosition);
		REGISTER_MONO_METHOD(Transform_SetRotation);
		REGISTER_MONO_METHOD(Transform_GetRotation);
		REGISTER_MONO_METHOD(Transform_SetScale);
		REGISTER_MONO_METHOD(Transform_GetScale);

		RegisterComponentGroup(ComponentGroup<Transform>{});
	}

	void Log_Trace(MonoString* in_msg)
	{
		Log::Trace("{}", ToString(in_msg));
	}

	void Log_Debug(MonoString* in_msg)
	{
		Log::Debug("{}", ToString(in_msg));
	}

	void Log_Error(MonoString* in_msg)
	{
		Log::Error("{}", ToString(in_msg));
	}

	void Log_Warn(MonoString* in_msg)
	{
		Log::Warn("{}", ToString(in_msg));
	}

	void Log_Info(MonoString* in_msg)
	{
		Log::Info("{}", ToString(in_msg));
	}

	bool Input_IsKeyPress(eKey in_key)
	{
		return Input::IsKeyPress(in_key);
	}

	bool Input_IsKeyRelease(eKey in_key)
	{
		return Input::IsKeyRelease(in_key);
	}

	bool Input_IsKeyDown(eKey in_key)
	{
		return Input::IsKeyDown(in_key);
	}

	bool Input_IsKeyUp(eKey in_key)
	{
		return Input::IsKeyUp(in_key);
	}

	MonoObject* Entity_TryCreateComponent(uint32 in_entityID, MonoReflectionType* in_componentType)
	{
		if (Scene* sc = GetSceneManager().TryGetCurrentScene())
		{
			Entity e = sc->TryFindByID(in_entityID);
			MonoType* type = mono_reflection_type_get_type(in_componentType);
			auto& map = GetMonoScriptBridge().GetEntityComponentMap();

			auto iter = map.find(type);
			if (iter != map.end())
			{
				return iter->second.tryCreateComponent(e).GetMonoObject();
			}
		}
		return nullptr;
	}

	MonoObject* Entity_CreateComponent(uint32 in_entityID, MonoReflectionType* in_componentType)
	{
		if (Scene* sc = GetSceneManager().TryGetCurrentScene())
		{
			Entity e = sc->TryFindByID(in_entityID);
			MonoType* type = mono_reflection_type_get_type(in_componentType);
			auto& map = GetMonoScriptBridge().GetEntityComponentMap();

			auto iter = map.find(type);
			if (iter != map.end())
			{
				return iter->second.createComponent(e).GetMonoObject();
			}
		}
		return nullptr;
	}

	MonoObject* Entity_ReplaceComponent(uint32 in_entityID, MonoReflectionType* in_componentType)
	{
		if (Scene* sc = GetSceneManager().TryGetCurrentScene())
		{
			Entity e = sc->TryFindByID(in_entityID);
			MonoType* type = mono_reflection_type_get_type(in_componentType);
			auto& map = GetMonoScriptBridge().GetEntityComponentMap();

			auto iter = map.find(type);
			if (iter != map.end())
			{
				return iter->second.replaceComponent(e).GetMonoObject();
			}
		}
		return nullptr;
	}

	bool Entity_HasComponent(uint32 in_entityID, MonoReflectionType* in_componentType)
	{
		if (Scene* sc = GetSceneManager().TryGetCurrentScene())
		{
			Entity e = sc->TryFindByID(in_entityID);
			MonoType* type = mono_reflection_type_get_type(in_componentType);
			auto& map = GetMonoScriptBridge().GetEntityComponentMap();

			auto iter = map.find(type);
			if (iter != map.end())
			{
				return iter->second.hasComponent(e);
			}
		}

		return false;
	}

	MonoObject* Entity_TryFindComponent(uint32 in_entityID, MonoReflectionType* in_componentType)
	{
		if (Scene* sc = GetSceneManager().TryGetCurrentScene())
		{
			Entity e = sc->TryFindByID(in_entityID);
			MonoType* type = mono_reflection_type_get_type(in_componentType);
			auto& map = GetMonoScriptBridge().GetEntityComponentMap();

			auto iter = map.find(type);
			if (iter != map.end())
			{
				return iter->second.tryFindComponent(e).GetMonoObject();
			}
		}
		return nullptr;
	}

	MonoObject* Entity_FindComponent(uint32 in_entityID, MonoReflectionType* in_componentType)
	{
		if (Scene* sc = GetSceneManager().TryGetCurrentScene())
		{
			Entity e = sc->TryFindByID(in_entityID);
			MonoType* type = mono_reflection_type_get_type(in_componentType);
			auto& map = GetMonoScriptBridge().GetEntityComponentMap();

			auto iter = map.find(type);
			if (iter != map.end())
			{
				return iter->second.findComponent(e).GetMonoObject();
			}
		}
		return nullptr;
	}

	void Entity_RemoveComponent(uint32 in_entityID, MonoReflectionType* in_componentType)
	{
		if (Scene* sc = GetSceneManager().TryGetCurrentScene())
		{
			Entity e = sc->TryFindByID(in_entityID);
			MonoType* type = mono_reflection_type_get_type(in_componentType);
			auto& map = GetMonoScriptBridge().GetEntityComponentMap();

			auto iter = map.find(type);
			if (iter != map.end())
			{
				iter->second.removeComponent(e);
			}
		}
	}

	bool IsValid(uint32 in_entityID)
	{
		if (Scene* sc = GetSceneManager().TryGetCurrentScene())
		{
			Entity e = sc->TryFindByID(in_entityID);
			return e.IsValid();
		}
		return false;
	}

	void Transform_SetPosition(uint32 in_entityID, const Vec3& in_position)
	{
		if (Scene* sc = GetSceneManager().TryGetCurrentScene())
		{
			Entity e = sc->TryFindByID(in_entityID);
			e.FindComponent<Transform>().position = in_position;
		}
	}

	void Transform_GetPosition(uint32 in_entityID, Vec3& out_position)
	{
		if (Scene* sc = GetSceneManager().TryGetCurrentScene())
		{
			Entity e = sc->TryFindByID(in_entityID);
			out_position = e.FindComponent<Transform>().position;
		}
	}

	void Transform_SetRotation(uint32 in_entityID, const Vec3& in_rotation)
	{
		if (Scene* sc = GetSceneManager().TryGetCurrentScene())
		{
			Entity e = sc->TryFindByID(in_entityID);
			e.FindComponent<Transform>().rotation = in_rotation;
		}
	}

	void Transform_GetRotation(uint32 in_entityID, Vec3& out_position)
	{
		if (Scene* sc = GetSceneManager().TryGetCurrentScene())
		{
			Entity e = sc->TryFindByID(in_entityID);
			out_position = e.FindComponent<Transform>().rotation;
		}
	}

	void Transform_SetScale(uint32 in_entityID, const Vec3& in_scaling)
	{
		if (Scene* sc = GetSceneManager().TryGetCurrentScene())
		{
			Entity e = sc->TryFindByID(in_entityID);
			e.FindComponent<Transform>().scaling = in_scaling;
		}
	}

	void Transform_GetScale(uint32 in_entityID, Vec3& out_scaling)
	{
		if (Scene* sc = GetSceneManager().TryGetCurrentScene())
		{
			Entity e = sc->TryFindByID(in_entityID);
			out_scaling = e.FindComponent<Transform>().scaling;
		}
	}
}