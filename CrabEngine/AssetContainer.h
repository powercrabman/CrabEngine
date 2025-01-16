#pragma once

namespace crab
{
	//===================================================
	//                    Container
	//===================================================

	template <typename AssetType>
	class AssetContainer
	{
		struct Slot;
	public:
		AssetContainer();
		~AssetContainer() = default;

		AssetContainer(const AssetContainer&) = delete;
		AssetContainer& operator=(const AssetContainer&) = delete;

		AssetID<AssetType> ReplaceOrCreate(Scope<AssetType>&& in_asset)
		{
			AssetID<AssetType> id = TryFindByName(in_asset->name);

			if (id != NullAsset<AssetType>)
			{
				Slot& slot = m_slots[id.index];
				slot.asset = std::move(in_asset);
				return id;
			}

			return Insert(std::move(in_asset));
		}

		AssetID<AssetType> Insert(Scope<AssetType>&& in_asset)
		{
			// 중복 확인
			if (IsContain(in_asset->name))
			{
				assert(false);
				return NullAsset<AssetType>;
			}

			uint32_t idx;

			if (!m_freeSlots.empty())
			{
				idx = m_freeSlots.back();
				m_freeSlots.pop_back();
			}
			else
			{
				idx = m_slots.size();
				m_slots.emplace_back();
			}

			m_slots[idx].asset = std::move(in_asset);
			m_slots[idx].token++;

			AssetID<AssetType> assetID{ idx, m_slots[idx].token };
			m_nameMap[m_slots[idx].asset->name] = assetID;
			m_assetList.emplace_back(assetID);

			return assetID;
		}

		bool Remove(AssetID<AssetType> in_handle)
		{
			Slot& slot = m_slots[in_handle.index];

			if (!slot.asset || in_handle.token != slot.token)
			{
				assert(false);
				return false;
			}

			m_nameMap.erase(slot.asset->name);
			auto it = std::find_if(m_assetList.begin(), m_assetList.end(), [&](const AssetID<AssetType>& in_other)
				{
					return in_other == in_handle;
				}
			);
			if (it != m_assetList.end()) { m_assetList.erase(it); }

			slot.asset.reset();
			m_freeSlots.push_back(in_handle.index);

			return true;
		}

		const AssetType* TryGetAsset(AssetID<AssetType> in_handle)
		{
			if (in_handle.index >= m_slots.size() || in_handle == NullAsset<AssetType>) return nullptr;

			Slot& slot = m_slots[in_handle.index];
			if (slot.asset && in_handle.token == slot.token)
			{
				return slot.asset.get();
			}
			return nullptr;
		}

		AssetType* TryGetAssetRef(AssetID<AssetType> in_handle)
		{
			if (in_handle.index >= m_slots.size() || in_handle == NullAsset<AssetType>) return nullptr;

			Slot& slot = m_slots[in_handle.index];
			if (slot.asset && in_handle.token == slot.token)
			{
				return slot.asset.get();
			}
			return nullptr;
		}

		AssetID<AssetType> TryFindByName(const std::string_view in_name)
		{
			auto iter = m_nameMap.find(in_name.data());

			if (iter != m_nameMap.end())
			{
				return iter->second;
			}

			return NullAsset<AssetType>;
		}

		bool TryRenameAsset(AssetID<AssetType> in_id, std::string_view in_newName)
		{
			AssetType* asset = TryGetAssetRef(in_id);

			if (asset->name == in_newName)
			{
				return true;
			}

			if (!asset)
			{
				assert(false);
				return false;
			}

			if (m_nameMap.contains(in_newName.data()))
			{
				assert(false);
				return false;
			}

			m_nameMap.erase(asset->name);
			asset->name = in_newName;
			auto[_, retval] = m_nameMap.emplace(in_newName, in_id);
			assert(retval);
			return retval;
		}

		bool IsContain(std::string_view in_name) const { return m_nameMap.count(in_name.data()); }
		void Clear();

		uint64 size() const { return m_assetList.size(); };
		auto cbegin() const { return m_assetList.cbegin(); }
		auto cend() const { return m_assetList.cend(); }

	private:
		struct Slot
		{
			Scope<AssetType> asset;
			uint32 token = 0;
		};

		std::vector<Slot>									m_slots;
		std::vector<uint32>									m_freeSlots;
		std::unordered_map<std::string, AssetID<AssetType>>	m_nameMap;
		std::vector<AssetID<AssetType>>						m_assetList;

		inline static constexpr uint32_t DEFAULT_CAPACITY = 255;
	};

	template <typename AssetType>
	void crab::AssetContainer<AssetType>::Clear()
	{
		m_slots.clear();
		m_freeSlots.clear();
		m_nameMap.clear();
		m_assetList.clear();
	}

	template <typename AssetType>
	crab::AssetContainer<AssetType>::AssetContainer()
	{
		m_slots.reserve(DEFAULT_CAPACITY);
		m_freeSlots.reserve(DEFAULT_CAPACITY);
		m_nameMap.reserve(DEFAULT_CAPACITY);
		m_assetList.reserve(DEFAULT_CAPACITY);
	}
}