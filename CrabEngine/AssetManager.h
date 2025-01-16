#pragma once

namespace crab
{
	class AssetManager : public Singleton<AssetManager>
	{
		friend class Singleton<AssetManager>;
	public:
		void Init(const ApplicationSetting& in_setting);
		void Shutdown();
		void LoadClientAssets();

		template <typename AssetType>
		AssetID<AssetType> EmplaceAsset(Scope<AssetType>&& in_asset)
		{
			m_dirtyBit = true;
			return get_asset_container<AssetType>().Insert(std::move(in_asset));
		}

		template <typename AssetType>
		AssetID<AssetType> ReplaceOrEmplaceAsset(Scope<AssetType>&& in_asset)
		{
			m_dirtyBit = true;
			return get_asset_container<AssetType>().ReplaceOrCreate(std::move(in_asset));
		}

		template <typename AssetType>
		bool RemoveAsset(AssetID<AssetType> in_handle)
		{
			m_dirtyBit = true;
			return get_asset_container<AssetType>().Remove(in_handle);
		}

		template <typename AssetType>
		AssetID<AssetType> TryFindAssetIDByName(const std::string_view in_name)
		{
			return get_asset_container<AssetType>().TryFindByName(in_name);
		}

		template <typename AssetType>
		const AssetType* TryGetAsset(AssetID<AssetType> in_handle)
		{
			return get_asset_container<AssetType>().TryGetAsset(in_handle);
		}

		template <typename AssetType>
		AssetType* TryGetAssetRef(AssetID<AssetType> in_handle)
		{
			m_dirtyBit = true;
			return get_asset_container<AssetType>().TryGetAssetRef(in_handle);
		}

		template <typename AssetType>
		bool	TryRenameAsset(AssetID<AssetType> in_id, std::string_view in_newName)
		{
			m_dirtyBit = true;
			return get_asset_container<AssetType>().TryRenameAsset(in_id, in_newName);
		}

		std::filesystem::path GetEngineDirectory() const { return m_engineDirectory; }
		std::filesystem::path GetClientDirectory() const { return m_projectDirectory; }

		std::filesystem::path GetEngineDirectory(const std::filesystem::path& in_path) const { return m_engineDirectory / in_path; }
		std::filesystem::path GetClientDirectory(const std::filesystem::path& in_path) const { return m_projectDirectory / in_path; }
		std::filesystem::path GetAssetDirectory(eAssetType in_type) const { return m_projectDirectory / "Assets" / ToString(in_type); }

		template<typename AssetType>
		auto GetBeginIterator() { return get_asset_container<AssetType>().cbegin(); }

		template<typename AssetType>
		auto GetEndIterator() { return get_asset_container<AssetType>().cend(); }

		template<typename AssetType>
		uint64 GetAssetContainerSize() { return get_asset_container<AssetType>().size(); }

		void SaveClientAssetsToFile();

		bool GetDirtyBit() const { return m_dirtyBit; }

	private:
		AssetManager() = default;
		~AssetManager() = default;

		template <typename AssetType>
		AssetContainer<AssetType>& get_asset_container();

		template<typename AssetType>
		void save_assets_to_file();
		void save_asset_to_file(const AssetBase* in_asset) const;

		void clear_client_asset_files() const;
		void clear_asset_containers();

		void load_client_asset(eAssetType in_type, 
			const std::vector<std::string_view>& in_extensions, 
			const std::function<void(const std::filesystem::path&)>& in_creationCallback = nullptr
		) const;

		std::filesystem::path		m_engineDirectory;
		std::filesystem::path		m_projectDirectory;

		AssetContainer<GameTexture>	m_gameTexRepo;
		AssetContainer<Flipbook>	m_flipbookRepo;
		AssetContainer<Sprite>		m_spriteRepo;
		AssetContainer<Mesh>		m_meshRepo;
		AssetContainer<MonoScript>	m_scriptRepo;
		bool						m_dirtyBit = false;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename AssetType>
	inline void AssetManager::save_assets_to_file()
	{
		auto iter = GetBeginIterator<AssetType>();
		auto end = GetEndIterator<AssetType>();

		for (; iter != end; ++iter)
		{
			const AssetType* asset = TryGetAsset(*iter);
			if (asset)
			{
				save_asset_to_file(asset);
				Log::Info(R"(Asset Save Done. "{}")", asset->name);
			}
			else
			{
				assert(false);
			}
		}
	}

	template <typename AssetType>
	crab::AssetContainer<AssetType>& crab::AssetManager::get_asset_container()
	{
		if constexpr (IS_SAME_TYPE(AssetType, Flipbook)) return m_flipbookRepo;
		else if constexpr (IS_SAME_TYPE(AssetType, Sprite)) return m_spriteRepo;
		else if constexpr (IS_SAME_TYPE(AssetType, Mesh)) return m_meshRepo;
		else if constexpr (IS_SAME_TYPE(AssetType, MonoScript)) return m_scriptRepo;
		else if constexpr (IS_SAME_TYPE(AssetType, GameTexture)) return m_gameTexRepo;
		else static_assert(false);
	}

	FORCEINLINE NODISCARD AssetManager& GetAssetManager()
	{
		return AssetManager::Get();
	}
}

