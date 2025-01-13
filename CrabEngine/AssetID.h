#pragma once

namespace crab
{
	template <typename AssetType>
	struct AssetID
	{
		bool operator==(const AssetID<AssetType>& in_other) const
		{
			return index == in_other.index && token == in_other.token;
		}

		bool operator!=(const AssetID<AssetType>& in_other) const
		{
			return !(*this == in_other);
		}

		uint32 index;
		uint32 token;
	};

	struct AnyAssetID
	{
		AnyAssetID() = default;
		AnyAssetID(uint32 in_index, uint32 in_token)
			: index(in_index)
			, token(in_token)
		{
		}

		void Clear() { index = UINT32_LIMIT; token = UINT32_LIMIT; }
		bool IsValid() const
		{
			return index != UINT32_LIMIT && token != UINT32_LIMIT;
		}

		bool IsSame(uint32 in_index, uint32 in_token) const
		{
			return index == in_index && token == in_token;
		}

		uint32 index;
		uint32 token;
	};

	template <typename AssetType>
	inline static AssetID<AssetType>	NullAsset = { UINT32_MAX, UINT32_MAX };
	inline static AnyAssetID			NullAnyAsset = { UINT32_MAX, UINT32_MAX };
}