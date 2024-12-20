#pragma once
#include "LayoutElement.h"

namespace crab
{
	class IShader
	{
	public:
		IShader() = default;
		virtual ~IShader() = default;

		virtual void Bind() = 0;

		static Ref<IShader> CreateByFile(
			const std::initializer_list<LayoutElement>& in_elements,
			const std::filesystem::path& in_vertexShaderPath,
			const std::filesystem::path& in_pixelShaderPath
		);

		static Ref<IShader> CreateByFile(
			const std::initializer_list<LayoutElement>& in_elements,
			const std::filesystem::path& in_shaderPath 
		);

		static Ref<IShader> CreateByString(
			const std::initializer_list<LayoutElement>& in_elements,
			const std::string_view in_vertexShader,
			const std::string_view in_pixelShader
		);

		static Ref<IShader> CreateByString(
			const std::initializer_list<LayoutElement>& in_elements,
			const std::string_view in_shader
		);

	private:
		virtual void _create_by_file_(
			const std::initializer_list<LayoutElement>& in_elements,
			const std::filesystem::path& in_vertexShaderPath,
			const std::filesystem::path& in_pixelShaderPath
		) = 0;

		virtual void _create_by_string_(
			const std::initializer_list<LayoutElement>& in_elements,
			const std::string_view& in_vertexShader,
			const std::string_view& in_pixelShader
		) = 0;
	};
}

