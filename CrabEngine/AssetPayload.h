#pragma once

namespace crab
{
	template<typename AssetType>
	void AssetPayloadTarget(AssetID<AssetType>& in_out_ID)
	{
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(fmt::format("{}Payload", ToString(AssetType::s_s_staticType)).c_str());
			if (payload)
			{
				in_out_ID = *static_cast<AssetID<AssetType>*>(payload->Data);
			}

			ImGui::EndDragDropTarget();
		}
	}

	template<typename AssetType>
	void AssetPayloadSource(AssetID<AssetType> in_ID)
	{
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::SetDragDropPayload(fmt::format("{}Payload", ToString(AssetType::s_s_staticType)).c_str(), &in_ID, sizeof(AssetID<AssetType>));
			ImGui::TextUnformatted(TryGetAsset(in_ID)->name.c_str());

			ImGui::EndDragDropSource();
		}
	}
}