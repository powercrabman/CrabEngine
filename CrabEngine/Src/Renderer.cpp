#include "CrabEnginePch.h"
#include "Renderer.h"
#include "DX11RenderAPI.h"

namespace crab
{
	void Renderer::Init(const ApplicationSetting& in_setting)
	{
		// Render API Factory
		switch (in_setting.renderingAPI)
		{
		case eRenderAPI::DirectX11:
			m_api = MakeScope<DX11RenderAPI>();
			break;
		default:
			ASSERT(false, "Unsupported Render API");
			return;
		}

		m_api->Init(in_setting);
		Draw2D::Init();
	}

	void Renderer::Shutdown()
	{
		m_api->Shutdown();
	}

	void Renderer::OnEvent(IEvent& in_event)
	{
		EventDispatcher dispatch{ in_event };

		dispatch.Dispatch<AppShutdown_Event>([](IEvent& in_event) { Renderer::Shutdown(); });

		dispatch.Dispatch<WindowResize_Event>([](IEvent& in_event)
			{
				WindowResize_Event& e = (WindowResize_Event&)in_event;
				m_api->OnResize(e.m_width, e.m_height);
			}
		);
	}

	void Renderer::BeginRender()
	{
		m_api->RenderBegin();
	}

	void Renderer::EndRender()
	{
		m_api->EndRender();
	}
}
