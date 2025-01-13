#pragma once

namespace crab
{
	class ComponentSerializerManager : public Singleton<ComponentSerializerManager>
	{
		using SerializeFn = std::function<void(Entity, Json&, void*)>;
		using DeserializeFn = std::function<void(Entity, const Json&)>;
	public:
		ComponentSerializerManager() {}
		~ComponentSerializerManager() {}

		static void Shutdown();

		template <typename Ty>
		void RegisterSerializerCallback()
		{
			ComponentSerializerHelper callbackFn;

			callbackFn.serializer = [](Entity in_entity, Json& in_json, void* in_data)
				{
					Ty* data = static_cast<Ty*>(in_data);
					in_json[std::to_string(in_entity.GetID())][Ty::s_staticName] = *data;
				};

			callbackFn.deserializer = [](Entity in_entity, const Json& in_json)
				{
					Ty data = in_json[std::to_string(in_entity.GetID())][Ty::s_staticName];
					in_entity.ReplaceComponent<Ty>(data);
				};

			entt::id_type id = entt::type_hash<Ty>::value();
			m_stringMapper.emplace(Ty::s_staticName, id);
			m_serialHelperRepo.emplace(id, callbackFn);
		}

		void CreateComponentFromJson(std::string_view in_compName, Entity in_entity, const Json& in_json)
		{
			if (m_stringMapper.contains(in_compName.data()))
			{
				m_serialHelperRepo[m_stringMapper[in_compName.data()]].deserializer(in_entity, in_json);
			}
			else
			{
				Log::Warn("{} is not deserialized.", in_compName.data());
			}
		}

		void ToJson(std::string_view in_compName, Entity in_entity, Json& in_json, void* in_data)
		{
			if (m_stringMapper.contains(in_compName.data()))
			{
				m_serialHelperRepo[m_stringMapper[in_compName.data()]].serializer(in_entity, in_json, in_data);
			}
			else
			{
				Log::Warn("{} is not serialized.", in_compName.data());
			}
		}

		void CreateComponentFromJson(entt::id_type in_id, Entity in_entity, const Json& in_json)
		{
			if (m_serialHelperRepo.contains(in_id))
			{
				m_serialHelperRepo[in_id].deserializer(in_entity, in_json);
			}
			else
			{
				Log::Warn("{} is not deserialized.", in_id);
			}
		}

		void ToJson(entt::id_type in_id, Entity in_entity, Json& in_json, void* in_data)
		{
			if (m_serialHelperRepo.contains(in_id))
			{
				m_serialHelperRepo[in_id].serializer(in_entity, in_json, in_data);
			}
			else
			{
				Log::Warn("{} is not serialized.", in_id);
			}
		}

	private:
		struct ComponentSerializerHelper
		{
			SerializeFn		serializer;
			DeserializeFn	deserializer;
		};

		std::unordered_map<entt::id_type, ComponentSerializerHelper>	m_serialHelperRepo;
		std::unordered_map<std::string, entt::id_type>					m_stringMapper;
	};
}
