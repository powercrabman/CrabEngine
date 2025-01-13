#pragma once
#include "IShader.h"

namespace crab
{
	class DX11Shader : public IShader
	{
	public:
		DX11Shader() = default;
		~DX11Shader() = default;

		void* GetNativePS() const override { return m_pShader.Get(); };
		void* GetNativeVS() const override { return m_vShader.Get(); };
		void* GetNativeLayout() const override { return m_layout.Get(); };

	private:
		void _create_by_file_(
			const std::initializer_list<LayoutElement>& in_elements,
			const std::filesystem::path& in_vertexShaderPath,
			const std::filesystem::path& in_pixelShaderPath
		) override;

		void _create_input_layout_(
			const std::initializer_list<crab::LayoutElement>& in_elements,
			const ComPtr<ID3DBlob>& vBlob
		);

		void _create_by_string_(
			const std::initializer_list<LayoutElement>& in_elements,
			const std::string_view& in_vertexShader,
			const std::string_view& in_pixelShader
		) override;

		::ComPtr<ID3DBlob> _load_and_compile_shader_or_null_(
			const std::filesystem::path& in_path,
			const std::string_view in_entryPoint,
			const std::string_view in_shaderModel
		);

		::ComPtr<ID3DBlob> _load_and_compile_shader_from_string_(
			const std::string_view& in_shaderCode,
			const std::string_view in_entryPoint,
			const std::string_view in_shaderModel
		);

	private:
		::ComPtr<ID3D11VertexShader> m_vShader;
		::ComPtr<ID3D11InputLayout> m_layout;

		::ComPtr<ID3D11PixelShader> m_pShader;
	};
}

