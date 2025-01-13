#pragma once

namespace crab
{
	class SceneSerializer : public JsonSerializerBase
	{
	public:
		void LoadSceneDataFromJson(Scene* in_scene);
		void ToJson(Scene* in_scene);
	};
}
