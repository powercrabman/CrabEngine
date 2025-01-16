#include "CrabEnginePch.h"
#include "DX11ImGuiAPI.h"

namespace crab
{
	namespace
	{
		struct ImGui_ImplDX11_Data
		{
			ID3D11Device* pd3dDevice;
			ID3D11DeviceContext* pd3dDeviceContext;
			IDXGIFactory* pFactory;
			ID3D11Buffer* pVB;
			ID3D11Buffer* pIB;
			ID3D11VertexShader* pVertexShader;
			ID3D11InputLayout* pInputLayout;
			ID3D11Buffer* pVertexConstantBuffer;
			ID3D11PixelShader* pPixelShader;
			ID3D11SamplerState* pFontSampler;
			ID3D11ShaderResourceView* pFontTextureView;
			ID3D11RasterizerState* pRasterizerState;
			ID3D11BlendState* pBlendState;
			ID3D11DepthStencilState* pDepthStencilState;
			int                         VertexBufferSize;
			int                         IndexBufferSize;

			ImGui_ImplDX11_Data() { memset((void*)this, 0, sizeof(*this)); VertexBufferSize = 5000; IndexBufferSize = 10000; }
		};

		struct ImGui_DX11RenderState
		{
			eBlendState blendState;
			eSamplerState samplerState;
			eDepthStencilState depthStencilState;
			eRasterizerState rasterizerState;
		};
	}

	void DX11ImGuiAPI::Init()
	{
		auto* api = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI());

		ImGui_ImplSDL2_InitForD3D(CrabEngine::Get().GetWindow().GetSDLWindow());
		ImGui_ImplDX11_Init(api->GetDevice().Get(), api->GetContext().Get());
	}

	void DX11ImGuiAPI::SetCurrentWindowDrawlistRenderState(eBlendState in_blendState, eSamplerState in_samplerState, eDepthStencilState in_depthStencilState, eRasterizerState in_rasterizerState)
	{
		ImGui_DX11RenderState state = {
			in_blendState,
			in_samplerState,
			in_depthStencilState,
			in_rasterizerState
		};

		ImGui::GetWindowDrawList()->AddCallback([](const ImDrawList* parent_list, const ImDrawCmd* cmd)
			{
				ImGui_DX11RenderState state = *(ImGui_DX11RenderState*)cmd->UserCallbackData;
				ImGui_ImplDX11_Data* bd = (ImGui_ImplDX11_Data*)ImGui::GetIO().BackendRendererUserData;
				ID3D11DeviceContext* device_ctx = bd->pd3dDeviceContext;

				unsigned int stride = sizeof(ImDrawVert);
				unsigned int offset = 0;
				device_ctx->IASetInputLayout(bd->pInputLayout);
				device_ctx->IASetVertexBuffers(0, 1, &bd->pVB, &stride, &offset);
				device_ctx->IASetIndexBuffer(bd->pIB, sizeof(ImDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
				device_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				device_ctx->VSSetShader(bd->pVertexShader, nullptr, 0);
				device_ctx->VSSetConstantBuffers(0, 1, &bd->pVertexConstantBuffer);
				device_ctx->PSSetShader(bd->pPixelShader, nullptr, 0);
				device_ctx->GSSetShader(nullptr, nullptr, 0);
				device_ctx->HSSetShader(nullptr, nullptr, 0);
				device_ctx->DSSetShader(nullptr, nullptr, 0);
				device_ctx->CSSetShader(nullptr, nullptr, 0);

				Renderer::BindBlendState(state.blendState);
				Renderer::BindDepthStencilState(state.depthStencilState);
				Renderer::BindRasterizerState(state.rasterizerState);
				Renderer::BindSamplerState(state.samplerState, 0);
			},
			&state,
			sizeof(ImGui_DX11RenderState)
		);
	}
}

