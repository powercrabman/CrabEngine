#include "CrabEnginePch.h"
#include "ImGuiRenderer.h"
#include "DX11ImGuiAPI.h"

namespace crab
{
	void ImGuiRenderer::Init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		switch (Renderer::GetRednerAPIType())
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

	void ImGuiRenderer::RenderBegin()
	{
		m_api->RenderBegin();

		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiRenderer::RenderEnd()
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

	void ImGuiRenderer::OnEvent(IEvent& in_event)
	{
		EventDispatcher dispatcher(in_event);

		dispatcher.Dispatch<AppShutdownEvent>([](IEvent& in_event) { Shutdown(); });
	}
}
