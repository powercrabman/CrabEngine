#pragma once

namespace crab
{
	inline void OnUpdateNativeScriptSystem(Scene* in_scene, TimeStamp& in_ts)
	{
		in_scene->GetView<NativeScriptRunner>().each([&](NativeScriptRunner& in_scriptRunner)
			{
				if (in_scriptRunner.script)
				{
					if (!in_scriptRunner.isInited)
					{
						in_scriptRunner.script->OnCreate();
						in_scriptRunner.isInited = true;
					}

					in_scriptRunner.script->OnUpdate(in_ts);
				}
			}
		);
	}
}
