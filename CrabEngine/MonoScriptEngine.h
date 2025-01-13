#pragma once
#include <mono/jit/jit.h>

namespace crab
{
	// Script Core DLL
	// User DLL (2)

	//===================================================
	//			        Mono Instance
	//===================================================

	class MonoInstance
	{
	public:
		MonoInstance() { *this = s_null; }
		MonoInstance(MonoClass* in_class, MonoObject* in_object)
			: m_monoClass(in_class), m_monoObject(in_object)
		{
		}

		void SetField(const char* in_fieldName, void* in_value) const
		{
			MonoClassField* field = mono_class_get_field_from_name(m_monoClass, in_fieldName);
			if (!field)
			{
				ASSERT(false, "field name is wrong");
				return;
			}

			mono_field_set_value(m_monoObject, field, in_value);
		}

		MonoMethod* CreateMethod(const char* in_methodName, int in_paramCount) const
		{
			return mono_class_get_method_from_name(m_monoClass, in_methodName, in_paramCount);
		}

		MonoClass* GetMonoClass() const { return m_monoClass; }
		MonoObject* GetMonoObject() const { return m_monoObject; }

		bool IsNull() const { return m_monoObject == s_null.m_monoObject && m_monoClass == s_null.m_monoClass; }
		static MonoInstance s_null;

	private:
		MonoClass* m_monoClass = nullptr;
		MonoObject* m_monoObject = nullptr;
	};

	//===================================================
	//			       Assembly Data
	//===================================================

	struct MonoAssemblyData
	{
		MonoInstance CreateMonoInstance(const char* in_namespace, const char* in_className) const
		{
			MonoClass* monoClass = mono_class_from_name(image, in_namespace, in_className);
			assert(monoClass);

			MonoObject* monoObject = mono_object_new(mono_domain_get(), monoClass);
			assert(monoObject);

			mono_runtime_object_init(monoObject);

			MonoInstance retVal{ monoClass, monoObject };
			return retVal;
		}

		MonoAssembly* assembly;
		MonoImage* image;
	};

	//===================================================
	//			         Mono Entity
	//===================================================

	struct MonoEntity
	{
		using OnCreateMethod = void(*)(MonoObject*, MonoException**);
		using OnUpdateMethod = void(*)(MonoObject*, float, MonoException**);

		void CallOnCreate()const
		{
			MonoException* exp = nullptr;
			onCreate(monoInstance.GetMonoObject(), &exp);
		}

		void CallOnUpdate(TimeStamp& in_ts) const
		{
			MonoException* exp = nullptr;
			onUpdate(monoInstance.GetMonoObject(), in_ts.deltaTime, &exp);
		}

		MonoInstance	monoInstance;

		OnCreateMethod		onCreate;
		OnUpdateMethod		onUpdate;
	};

	//===================================================
	//			     Mono Script Engine
	//===================================================

	class MonoScriptEngine
	{
	public:
		static void Init(const ApplicationSetting& in_setting);
		static void Shutdown();

		static void OnEnterScene();
		static void OnUpdate(TimeStamp in_ts);

		static MonoAssemblyData LoadAssembly(const std::filesystem::path& in_path);
		static MonoDomain*		GetCoreDomain() { return m_coreDomain; }

		static MonoAssemblyData GetCoreAssemblyData() { return m_coreAssem; }
		static MonoAssemblyData GetUserAssemblyData() { return m_userAssem; }

	private:
		inline static MonoDomain* m_coreDomain = nullptr;
		inline static MonoAssemblyData m_coreAssem = {};
		inline static MonoAssemblyData m_userAssem = {};

		inline static std::filesystem::path m_coreScriptDLLPath;
		inline static std::filesystem::path m_userScriptDLLPath;
	};
}

