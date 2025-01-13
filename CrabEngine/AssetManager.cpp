#include "CrabEnginePch.h"
#include "AssetManager.h"
#include "AssetSerializer.h"

namespace crab
{
	void AssetManager::Init(const ApplicationSetting& in_setting)
	{
		m_engineDirectory = in_setting.engineDirectory;
		m_projectDirectory = in_setting.projectDirectory;

		// Asset Directory Creation
		for (uint32 i = 0; i < eAssetTypeCount; ++i)
		{
			std::filesystem::create_directories(GetAssetDirectory(static_cast<eAssetType>(i)));
		}
	}

	void AssetManager::Shutdown()
	{
		Destroy();
	}

	void AssetManager::LoadClientAssets()
	{
		std::filesystem::path curPath = m_projectDirectory / "Assets";

		for (const auto& entry : std::filesystem::recursive_directory_iterator(curPath))
		{
			const std::filesystem::path filepath = entry.path();
			const std::filesystem::path extension = filepath.extension();

			Log::Debug("{}", filepath.string());

			if(extension == ".json")
			{
				AssetSerializer serializer;
				serializer.LoadJsonFromFile(filepath);
				serializer.CreateFromJson();
			}
			else if (extension == ".bmp" || extension == ".jpeg" || extension == ".png" || extension == ".jpg")
			{
				GameTexture::Create(filepath.filename().stem().string(), filepath);
			}
		}
	}

}
