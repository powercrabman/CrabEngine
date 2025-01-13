#pragma once

#include "DirectXTex\DirectXTex\DirectXTex.h"
#include "DirectXTex\DirectXTex\DirectXTex.inl"
#include "ITexture.h"

namespace crab
{
	class DX11Texture : public ITexture
	{
	public:
		void*							GetNativeTexture() const override { return m_srv.Get(); }
		std::pair<float, float>			GetSize() const override { return std::make_pair((float)m_metaData.width, (float)m_metaData.height); }
		float							GetAspect() const override { return static_cast<float>(m_metaData.width) / static_cast<float>(m_metaData.height); }
		std::wstring_view				GetName() const override { return m_name; }
		const std::filesystem::path&	GetPath() const override { return m_path; }

		::ComPtr<ID3D11Texture2D>		GetDX11Texture() const { return m_tex; }

	private:
		void _create_(const std::filesystem::path& in_path) override;

		::ComPtr<ID3D11ShaderResourceView>	m_srv = nullptr;
		::ComPtr<ID3D11Texture2D>			m_tex = nullptr;
		DirectX::TexMetadata				m_metaData = {};
		DirectX::ScratchImage				m_image = {};
		std::wstring						m_name = {};
		std::filesystem::path				m_path = {};
	};
}