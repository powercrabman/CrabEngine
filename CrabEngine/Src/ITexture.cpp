#include "CrabEnginePch.h"
#include "ITexture.h"
#include "DX11Texture.h"

namespace crab
{
	Ref<ITexture> ITexture::Create(const std::filesystem::path& in_path)
	{
		Ref<ITexture> texture = nullptr;

		switch (Renderer::GetRenderAPIType())
		{
		case eRenderAPI::DirectX11:
			texture = MakeRef<DX11Texture>();
			break;
		default: ASSERT(false, "this api is not available.");
		}

		texture->_create_(in_path);
		return texture;
	}
}
