#include "CrabEnginePch.h"
#include "DX11Texture.h"
#include "DX11RenderAPI.h"

namespace crab
{

	void DX11Texture::_create_(const std::filesystem::path& in_path)
	{
		DX11RenderAPI* api = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI());
		::ComPtr<ID3D11Device> device = api->GetDevice();

		std::wstring texturename = in_path.filename().stem();
		std::string textureString = crab::str::CvtToString(texturename);
		Log::Trace(R"(image file load "{0}")", textureString);

		// Load Image
		HR hr = {};
		hr = DirectX::LoadFromWICFile(
			in_path.c_str(),
			DirectX::WIC_FLAGS_NONE,
			&m_metaData,
			m_image,
			nullptr
		);

		DX_ASSERT(hr, R"(image file load fail "{0}")", textureString);

		// Create Texture
		::ComPtr<ID3D11Resource> res = nullptr;
		hr = DirectX::CreateTexture(
			device.Get(),
			m_image.GetImages(),
			m_image.GetImageCount(),
			m_metaData,
			res.GetAddressOf()
		);
		DX_ASSERT(hr, R"(image file load fail "{0}")", textureString);

		DX_ASSERT(res.As(&m_tex), R"(image file load fail "{0}")", textureString);

		// Create SRV
		hr = DirectX::CreateShaderResourceView(
			device.Get(),
			m_image.GetImages(),
			m_image.GetImageCount(),
			m_metaData,
			m_srv.GetAddressOf()
		);
		DX_ASSERT(hr, "Create Shader Resource View Fail.");

		m_name = texturename;
		m_path = in_path;
		Log::Info(R"(image file load done "{}")", textureString);
	}
}