#include "CrabEnginePch.h"
#include "ImGuiRenderer.h"
#include "DX11ImGuiAPI.h"

namespace crab
{
	void ImGuiRenderer::Init(const ApplicationSetting& in_setting)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		switch (in_setting.renderingAPI)
		{

		case eRenderAPI::DirectX11:
			m_api = MakeScope<DX11ImGuiAPI>();
			break;

		default:
			ASSERT(false, "Not supported rendering API.");
			break;
		}

		m_api->Init();
	}

	void ImGuiRenderer::Shutdown()
	{
		m_api->Shutdown();

		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiRenderer::BeginRender()
	{
		m_api->RenderBegin();

		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		Input::UpdateInputState(); // imgui의 IO를 이용하는 방식이기 때문 -> 현재는 계획에 없지만 바꿀 수도 있음
	}

	void ImGuiRenderer::EndRender()
	{
		ImGui::Render();
		m_api->RenderEnd();

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	}
