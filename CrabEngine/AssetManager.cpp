#include "CrabEnginePch.h"
#include "AssetManager.h"
#include "AssetSerializer.h"

namespace crab
{
	void AssetManager::Init(const ApplicationSetting& in_setting)
	{
		m_engineDirectory = in_setting.engineDirectory;
		m_projectDirectory = in_setting.projectDirectory;
		m_dirtyBit = false;

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
		clear_asset_containers();
		std::filesystem::path curPath = m_projectDirectory / "Assets";

		// GameTexture 먼저 우선적으로 로딩해야함
		auto GameTextureCreator = [](const std::filesystem::path& in_path) 
			{
				GameTexture::Create(in_path.filename().stem().string(), ITexture::Create(in_path));
			};
		load_client_asset(eAssetType::GameTexture, { ".png", ".jpg", ".jpeg", ".bmp", ".gif" }, GameTextureCreator);
		load_client_asset(eAssetType::Flipbook, { ".json" });
		load_client_asset(eAssetType::Sprite, { ".json" });
		//load_client_asset(eAssetType::MonoScript, { "???" });
		//load_client_asset(eAssetType::Mesh, { "???" });

		m_dirtyBit = false;
	}

	void AssetManager::SaveClientAssetsToFile()
	{
		// TODO: Mono Script ..
		m_dirtyBit = false;
		clear_client_asset_files();
		save_assets_to_file<Sprite>();
		save_assets_to_file<Flipbook>();
	}

	void AssetManager::save_asset_to_file(const AssetBase* in_asset) const
	{
		AssetSerializer serializer;
		serializer.SaveToJson(in_asset);
		serializer.SaveJsonToFile(GetAssetDirectory(in_asset->type) / in_asset->name);
	}

	void AssetManager::clear_client_asset_files() const
	{
		std::filesystem::path curPath = m_projectDirectory / "Assets";

		for (const auto& entry : std::filesystem::recursive_directory_iterator(curPath))
		{
			if (entry.is_directory())
			{
				const auto& filename = entry.path().filename().stem();
				if (filename != ToString(eAssetType::GameTexture))
				{
					std::filesystem::remove_all(entry.path());
					std::filesystem::create_directory(entry.path());
				}
			}
		}
	}

	void AssetManager::clear_asset_containers()
	{
		m_gameTexRepo.Clear();
		m_flipbookRepo.Clear();
		m_spriteRepo.Clear();
		m_meshRepo.Clear();
		m_scriptRepo.Clear();
	}

	void AssetManager::load_client_asset(
		eAssetType in_type,
		const std::vector<std::string_view>& in_extensions,
		const std::function<void(const std::filesystem::path&)>& in_creationCallback
	) const
	{
		std::filesystem::path path = GetAssetDirectory(in_type);

		for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
		{
			const auto& filename = entry.path().filename().stem();
			const auto& extension = entry.path().filename().extension();

			for (const std::string_view filter : in_extensions)
			{
				if (filter == extension)
				{
					if (in_creationCallback)
					{
						in_creationCallback(entry.path());
					}
					else
					{
						AssetSerializer serializer;
						serializer.LoadJsonFromFile(entry.path());
						serializer.CreateFromJson();
					}
				}
			}
		}
	}
}
