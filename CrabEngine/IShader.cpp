#include "CrabEnginePch.h"
#include "IShader.h"
#include "DX11Shader.h"

namespace crab
{
	Ref<IShader> IShader::CreateByFile(const std::initializer_list<LayoutElement>& in_elements, const std::filesystem::path& in_vertexShaderPath, const std::filesystem::path& in_pixelShaderPath)
	{
		Ref<IShader> shader;
		switch (Renderer::GetRenderAPIType())
		{

		case eRenderAPI::DirectX11:
			shader = MakeRef<DX11Shader>();
			break;
		default: assert(false); return nullptr;
		}
		
		shader->_create_by_file_(in_elements, in_vertexShaderPath, in_pixelShaderPath);
		return shader;
	}

	Ref<IShader> IShader::CreateByFile(const std::initializer_list<LayoutElement>& in_elements, const std::filesystem::path& in_shaderPath)
	{
		return CreateByFile(in_elements, in_shaderPath, in_shaderPath);
	}

	Ref<IShader> IShader::CreateByString(const std::initializer_list<LayoutElement>& in_elements, const std::string_view in_vertexShader, const std::string_view in_pixelShader)
	{
		Ref<IShader> shader;
		switch (Renderer::GetRenderAPIType())
		{

		case eRenderAPI::DirectX11:
			shader = MakeRef<DX11Shader>();
			break;
		default: assert(false); return nullptr;
		}

		shader->_create_by_string_(in_elements, in_vertexShader, in_pixelShader);
		return shader;
	}

	Ref<IShader> IShader::CreateByString(const std::initializer_list<LayoutElement>& in_elements, const std::string_view in_shader)
	{
		return CreateByString(in_elements, in_shader, in_shader);
	}
}

