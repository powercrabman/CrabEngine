#pragma once

#include "DirectXTex\DirectXTex\DirectXTex.h"
#include "DirectXTex\DirectXTex\DirectXTex.inl"
#include "ITexture.h"

namespace crab
{
	class DX11Texture : public ITexture
	{
	public:
		DX11Texture() = default;
		~DX11Texture() = default;

		void					Bind(const uint32_t in_slot) const override;

		void*					GetNativeTexture() const override { return m_srv.Get(); }
		std::pair<float, float>	GetSize() const override { return std::make_pair(m_metaData.width, m_metaData.height); }
		float					GetAspect() const override { return static_cast<float>(m_metaData.width) / static_cast<float>(m_metaData.height); }
		std::wstring_view		GetName() const override { return m_name; }

		::ComPtr<ID3D11Texture2D> GetDX11Texture() const { return m_tex; }

	private:
		void _create_(const std::filesystem::path& in_path) override;

		::ComPtr<ID3D11ShaderResourceView>	m_srv = nullptr;
		::ComPtr<ID3D11Texture2D>			m_tex = nullptr;
		DirectX::TexMetadata				m_metaData = {};
		DirectX::ScratchImage				m_image = {};
		std::wstring						m_name = {};
	};
}