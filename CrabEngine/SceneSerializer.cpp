#include "CrabEnginePch.h"
#include "SceneSerializer.h"

namespace crab
{
	void SceneSerializer::LoadSceneDataFromJson(Scene* in_scene)
	{
		Log::Info("Load Scene Data: {} (path: {})", in_scene->ToString(), in_scene->GetSceneDataPath().string());

		in_scene->ClearScene();
		entt::registry& regi = in_scene->GetRegistry();
		assert(in_scene->ToString() == m_json["scene name"]);

		for (const auto& item : m_json.items())
		{
			if (item.key() == "scene name") continue;

			// entity의 GetTypeID 라고 가정
			auto [ret, id] = str::stoi(item.key());
			assert(ret);

			entt::entity ent = static_cast<entt::entity>(id);
			if (!regi.valid(ent))
			{
				ent = regi.create(ent);
			}

			Entity entity{ regi,  ent };
			for (const auto& cmpItem : item.value().items())
			{
				ComponentSerializerManager::Get().CreateComponentFromJson(cmpItem.key(), entity, m_json);
			}
		}
	}

	void SceneSerializer::ToJson(Scene* in_scene)
	{
		Log::Info("Save Scene Data: {} (path: {})", in_scene->ToString(), in_scene->GetSceneDataPath().string());

		entt::registry& regi = in_scene->GetRegistry();
		m_json["scene name"] = in_scene->ToString();

		auto storage = regi.storage();
		for (auto [id, set] : storage)
		{
			for (auto entity : set)
			{
				ComponentSerializerManager::Get().ToJson(
					id,
					Entity{ regi, entity },
					m_json,
					set.value(entity)
				);
			}
		}
	}
}

