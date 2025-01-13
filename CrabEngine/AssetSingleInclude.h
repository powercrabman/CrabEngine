#pragma once

#include "AssetID.h"
#include "Assets.h"
#include "AssetContainer.h"
#include "AssetManager.h"

// helper function
namespace crab
{
	template <typename AssetType>
	const AssetType* TryGetAsset(AssetID<AssetType> in_id)
	{
		return GetAssetManager().TryGetAsset(in_id);
	}

	template <typename AssetType>
	AssetType* TryGetAssetRef(AssetID<AssetType> in_id)
	{
		return GetAssetManager().TryGetAssetRef(in_id);
	}

	template <typename AssetType>
	bool IsValidAsset(AssetID<AssetType> in_id)
	{
		return TryGetAsset(in_id);
	}
}