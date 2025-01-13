#pragma once
   
namespace crab
{
#define EVENT_IMPLEMENT(EventType) \
	eEventType	GetType() override {return eEventType::EventType;}\
	inline static eEventType StaticType = eEventType::EventType;\
	inline static const char* EventString = #EventType         

	enum class eEventType
	{
		AppShutdown_Event,
		AppClose_Event,
		WindowResize_Event,
		WindowMove_Event,

		// In Editor
		SetEditorState_EditorEvent,
		NextToFrame_EditorEvent,
		DeleteEntity_EditorEvent,
		DuplicateEntity_EditorEvent,
		SaveScene_EditorEvent,
		ReloadScene_EditorEvent,
		SetGuizmoType_EditorEvent
	};

	class IEvent
	{
	public:
		virtual ~IEvent() = default;
		virtual std::string ToString() = 0;
		virtual eEventType	GetType() = 0;

		bool m_isHandled = false;
	};

	class WindowMove_Event : public IEvent
	{
	public:
		EVENT_IMPLEMENT(WindowMove_Event);
		std::string ToString() override
		{
			return fmt::format("{} : ({}, {})", EventString, m_x, m_y);
		}

		uint32 m_x;
		uint32 m_y;
	};

	class WindowResize_Event : public IEvent
	{
	public:
		EVENT_IMPLEMENT(WindowResize_Event);
		std::string ToString() override
		{
			return fmt::format("{} : ({}, {})", EventString, m_width, m_height);
		}

		uint32 m_width;
		uint32 m_height;
	};

	class AppShutdown_Event : public IEvent
	{
	public:
		std::string ToString() override { return EventString; }

		EVENT_IMPLEMENT(AppShutdown_Event);
	};

	class AppClose_Event : public IEvent
	{
	public:
		std::string ToString() override { return EventString; }

		EVENT_IMPLEMENT(AppClose_Event);
	};

	enum class eMouse
	{
		Left = 0,
		Right,
		Middle
	};

	//===================================================
	//                      INPUT
	//===================================================

	constexpr const char* ToString(eMouse in_e)
	{
		switch (in_e)
		{
		case crab::eMouse::Left:   return "Left";
		case crab::eMouse::Middle: return "Middle";
		case crab::eMouse::Right:  return "Right";
		default: assert(false); return "";
		}
	}
}

