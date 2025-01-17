#include "CrabEnginePch.h"
#include "MonoScriptEngine.h"
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/threads.h>

#include "MonoScriptBridge.h"

namespace crab
{
	MonoInstance MonoInstance::s_null = { nullptr, nullptr };

	void MonoScriptEngine::Init(const ApplicationSetting& in_setting)
	{
		m_userScriptDLLPath = in_setting.projectScriptDirectory;
		m_coreScriptDLLPath = in_setting.scriptEngineDirectory;

#if defined(DEBUG) || defined(_DEBUG)
		m_coreScriptDLLPath /= R"(bin\Debug\CrabScriptCore.dll)";

		m_userScriptDLLPath /= R"(bin\Debug)";
		m_userScriptDLLPath /= fmt::format("{}Script.dll", in_setting.applicationName);
#else
		m_coreScriptDLLPath /= R"(bin\Release\CrabScriptCore.dll)";

		m_userScriptDLLPath /= R"(bin\Release)";
		m_userScriptDLLPath /= fmt::format("{}Script.dll", in_setting.applicationName);
#endif
		
		m_coreDomain = mono_jit_init(in_setting.applicationName.c_str());
		mono_config_parse(nullptr);

		m_coreAssem = LoadAssembly(m_coreScriptDLLPath);
		m_userCSProjPath = in_setting.projectScriptDirectory;

		GetMonoScriptBridge().Init();
		BuildGameScript();
	}

	void MonoScriptEngine::Shutdown()
	{
		MonoScriptBridge::Destroy();
		mono_jit_cleanup(m_coreDomain);
	}

	MonoAssemblyData MonoScriptEngine::LoadAssembly(const std::filesystem::path& in_path)
	{
		MonoAssemblyData assem;
		std::string path = in_path.string();
		assem.assembly = mono_domain_assembly_open(GetCoreDomain(), path.c_str());
		ASSERT(assem.assembly, "Assembly Load Fail. [{}]", path);

		assem.image = mono_assembly_get_image(assem.assembly);
		ASSERT(assem.image, "Image Load Fail. [{}]", path);

		return assem;
	}

	void MonoScriptEngine::BuildGameScript()
	{
		// 프로젝트의 C# 솔루션 빌드
		std::string command = fmt::format("cd {} && dotnet build", m_userCSProjPath.string());
		int result = system(command.c_str());
		if (result == 0)
		{
			Log::Info("create C# .dll build done.");
			m_isRunning = true;
			m_userAssem = LoadAssembly(m_userScriptDLLPath);
		}
		else
		{
			assert(false);
			Log::Error("create C# .dll build fail.");
			m_isRunning = false;
			return;
		}
	}

	void MonoScriptEngine::OnEnterScene()
	{
		auto view = GetSceneManager().TryGetCurrentScene()->GetView<MonoScriptRunner>();

		for (auto entity : view)
		{
			MonoScriptRunner& scriptRunner = view.get<MonoScriptRunner>(entity);
			MonoScript* script = TryGetAssetRef(scriptRunner.assetID);
			if (script)
			{
				MonoEntity& monoEntity = script->internalData;
				MonoInstance& monoInstance = monoEntity.monoInstance;

				// C# Object 생성
				monoInstance = m_userAssem.CreateMonoInstance(script->namespaceName.c_str(), script->className.c_str());

				uint32 id = static_cast<uint32>(entity);
				monoInstance.SetField("ID", &id);

				// 메서드 바인딩
				MonoMethod* ctorFn = monoInstance.CreateMethod(".ctor", 0);
				MonoMethod* onCreateFn = monoInstance.CreateMethod("OnCreate", 0);
				MonoMethod* onUpdateFn = monoInstance.CreateMethod("OnUpdate", 1);

				//monoEntity.constructor = static_cast<MonoEntity::ConstructorMethod>(mono_method_get_unmanaged_thunk(ctorFn));
				monoEntity.onCreate = static_cast<MonoEntity::OnCreateMethod>(mono_method_get_unmanaged_thunk(onCreateFn));
				monoEntity.onUpdate = static_cast<MonoEntity::OnUpdateMethod>(mono_method_get_unmanaged_thunk(onUpdateFn));

				// 생성자 및 Init 함수 호출
				monoEntity.CallOnCreate();
			}
		}
	}

	void MonoScriptEngine::OnUpdate(TimeStamp in_ts)
	{
		if (!m_isRunning)
			return;

		GetSceneManager().TryGetCurrentScene()->GetView<MonoScriptRunner>().each([&](MonoScriptRunner& in_sr)
			{
				MonoScript* script = TryGetAssetRef(in_sr.assetID);
				if (script)
				{
					script->internalData.CallOnUpdate(in_ts);
				}
			}
		);
	}

}
