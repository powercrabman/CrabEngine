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
			return _get_asset_container_<AssetType>().Insert(std::move(in_asset));
		}

		template <typename AssetType>
		AssetID<AssetType> ReplaceOrEmplaceAsset(Scope<AssetType>&& in_asset)
		{
			return _get_asset_container_<AssetType>().ReplaceOrInsert(std::move(in_asset));
		}

		template <typename AssetType>
		bool RemoveAsset(AssetID<AssetType> in_handle)
		{
			return _get_asset_container_<AssetType>().Remove(in_handle);
		}

		template <typename AssetType>
		AssetID<AssetType> TryFindAssetIDByName(const std::string_view in_name)
		{
			return _get_asset_container_<AssetType>().TryFindByName(in_name);
		}

		template <typename AssetType>
		const AssetType* TryGetAsset(AssetID<AssetType> in_handle)
		{
			return _get_asset_container_<AssetType>().TryGetAsset(in_handle);
		}

		template <typename AssetType>
		AssetType* TryGetAssetRef(AssetID<AssetType> in_handle)
		{
			return _get_asset_container_<AssetType>().TryGetAssetRef(in_handle);
		}

		std::filesystem::path GetEngineDirectory() const { return m_engineDirectory; }
		std::filesystem::path GetClientDirectory() const { return m_projectDirectory; }

		std::filesystem::path GetEngineDirectory(const std::filesystem::path& in_path) const { return m_engineDirectory / in_path; }
		std::filesystem::path GetClientDirectory(const std::filesystem::path& in_path) const { return m_projectDirectory / in_path; }
		std::filesystem::path GetAssetDirectory(eAssetType in_type) const { return m_projectDirectory / "Assets" / ToString(in_type); }

		template<typename AssetType>
		auto GetBeginIterator() { return _get_asset_container_<AssetType>().cbegin(); }

		template<typename AssetType>
		auto GetEndIterator() { return _get_asset_container_<AssetType>().cend(); }

		template<typename AssetType>
		uint64 GetAssetContainerSize() { return _get_asset_container_<AssetType>().size(); }

	private:
		AssetManager() = default;
		~AssetManager() = default;

		template <typename AssetType>
		AssetContainer<AssetType>& _get_asset_container_();

		std::filesystem::path		m_engineDirectory;
		std::filesystem::path		m_projectDirectory;

		AssetContainer<GameTexture>	m_gameTexRepo;
		AssetContainer<Flipbook>	m_flipbookRepo;
		AssetContainer<Sprite>		m_spriteRepo;
		AssetContainer<Mesh>		m_meshRepo;
		AssetContainer<MonoScript>	m_scriptRepo;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename AssetType>
	crab::AssetContainer<AssetType>& crab::AssetManager::_get_asset_container_()
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

