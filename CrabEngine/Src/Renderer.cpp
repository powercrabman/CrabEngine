#include "CrabEnginePch.h"
#include "Renderer.h"
#include "DX11RenderAPI.h"

namespace crab
{
	void Renderer::Init(eRenderAPI in_api, const int in_width, const int in_height)
	{
		// Render API Factory
		switch (in_api)
		{
		case eRenderAPI::DirectX11:
			m_api = MakeScope<DX11RenderAPI>();
			break;
		default:
			ASSERT(false, "Unsupported Render API");
			return;
		}

		m_api->Init(in_width, in_height);
	}

	void Renderer::Shutdown()
	{
		m_api->Shutdown();
	}

	void Renderer::OnEvent(IEvent& in_event)
	{
		EventDispatcher dispatch{ in_event };

		dispatch.Dispatch<AppShutdownEvent>([](IEvent& in_event) { Renderer::Shutdown(); });

		dispatch.Dispatch<WindowResizeEvent>([](IEvent& in_event)
			{
				WindowResizeEvent& e = (WindowResizeEvent&)in_event;
				m_api->OnResize(e.m_width, e.m_height);
			}
		);
	}
}