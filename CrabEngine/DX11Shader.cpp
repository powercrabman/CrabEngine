#include "CrabEnginePch.h"
#include "DX11Shader.h"

namespace crab
{
	void DX11Shader::_create_by_file_(const std::initializer_list<LayoutElement>& in_elements, const std::filesystem::path& in_vertexShaderPath, const std::filesystem::path& in_pixelShaderPath)
	{
		auto device = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();
		HR hr = {};

		// Create VertexShader
		::ComPtr<ID3DBlob> vBlob = _load_and_compile_shader_or_null_(in_vertexShaderPath, "VSmain", "vs_5_0");
		if (vBlob == nullptr)
		{
			Log::Error("vertex shader creation fail");
			return;
		}

		DX_ASSERT(device->CreateVertexShader(vBlob->GetBufferPointer(), vBlob->GetBufferSize(), nullptr, m_vShader.GetAddressOf()), "vertexshader create fail.");

		// Create InputLayout
		_create_input_layout_(in_elements, vBlob);

		// Create PixelShader
		::ComPtr<ID3DBlob> pBlob = _load_and_compile_shader_or_null_(in_pixelShaderPath, "PSmain", "ps_5_0");
		if (pBlob == nullptr)
		{
			Log::Error("pixel shader creation fail");
			return;
		}

		DX_ASSERT(device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_pShader.GetAddressOf()), "pixel shader create fail.");
	}

	void DX11Shader::_create_input_layout_(const std::initializer_list<crab::LayoutElement>& in_elements, const ComPtr<ID3DBlob>& vBlob)
	{
		// Create Input layout
		auto device = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();

		std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
		elements.reserve(in_elements.size());
		for (const auto& elem : in_elements)
		{
			D3D11_INPUT_ELEMENT_DESC desc = {};
			desc.SemanticName = elem.name.c_str();
			desc.SemanticIndex = elem.index;
			desc.InputSlot = 0;
			desc.Format = CvtDataFormatToDXGIFormat(elem.format);
			desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;

			elements.push_back(desc);
		}

		DX_ASSERT(device->CreateInputLayout(elements.data(), elements.size(), vBlob->GetBufferPointer(), vBlob->GetBufferSize(), m_layout.GetAddressOf()), "create inputlayout fail.");
	}

	::ComPtr<ID3DBlob> DX11Shader::_load_and_compile_shader_or_null_(const std::filesystem::path& in_path, const std::string_view in_entryPoint, const std::string_view in_shaderModel)
	{
		HR hr = {};

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(_DEBUG) | defined(DEBUG)
		dwShaderFlags |= D3DCOMPILE_DEBUG;
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		::ComPtr<ID3DBlob> errorBlob = nullptr;
		::ComPtr<ID3DBlob> shaderBlob = nullptr;

		hr = D3DCompileFromFile(
			in_path.c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			in_entryPoint.data(),
			in_shaderModel.data(),
			dwShaderFlags,
			0,
			shaderBlob.GetAddressOf(),
			errorBlob.GetAddressOf()
		);

		if (FAILED(hr))
		{
			if (errorBlob)
			{
				Log::Error("{}", static_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			DX_ASSERT(hr, "Shader Compile fail!");
		}

		return shaderBlob;
	}

	::ComPtr<ID3DBlob> DX11Shader::_load_and_compile_shader_from_string_(const std::string_view& in_shaderCode, const std::string_view in_entryPoint, const std::string_view in_shaderModel)
	{
		HR hr = {};

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(_DEBUG) | defined(DEBUG)
		dwShaderFlags |= D3DCOMPILE_DEBUG;
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		::ComPtr<ID3DBlob> shaderBlob = nullptr;
		::ComPtr<ID3DBlob> errorBlob = nullptr;

		hr = D3DCompile(
			in_shaderCode.data(),
			in_shaderCode.size(),
			nullptr,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			in_entryPoint.data(),
			in_shaderModel.data(),
			dwShaderFlags,
			0,
			shaderBlob.GetAddressOf(),
			errorBlob.GetAddressOf()
		);

		if (FAILED(hr))
		{
			if (errorBlob)
			{
				Log::Error("{}", static_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			DX_ASSERT(hr, "Shader Compile fail!");
		}

		return shaderBlob;
	}

	void DX11Shader::_create_by_string_(const std::initializer_list<LayoutElement>& in_elements, const std::string_view& in_vertexShader, const std::string_view& in_pixelShader)
	{
		auto device = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();
		HR hr = {};

		// Create VertexShader
		::ComPtr<ID3DBlob> vBlob = _load_and_compile_shader_from_string_(in_vertexShader, "VSmain", "vs_5_0");
		if (vBlob == nullptr)
		{
			Log::Error("vertex shader creation fail");
			return;
		}

		DX_ASSERT(device->CreateVertexShader(vBlob->GetBufferPointer(), vBlob->GetBufferSize(), nullptr, m_vShader.GetAddressOf()), "vertexshader create fail.");

		// Create InputLayout
		_create_input_layout_(in_elements, vBlob);

		// Create PixelShader
		::ComPtr<ID3DBlob> pBlob = _load_and_compile_shader_from_string_(in_pixelShader, "PSmain", "ps_5_0");
		if (pBlob == nullptr)
		{
			Log::Error("pixel shader creation fail");
			return;
		}

		DX_ASSERT(device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_pShader.GetAddressOf()), "pixel shader create fail.");
	}
}
