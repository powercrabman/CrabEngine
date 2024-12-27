#pragma once
   
namespace crab
{
#define EVENT_IMPLEMENT(EventType) \
	eEventType	GetType() override {return eEventType::EventType;}\
	inline static eEventType StaticType = eEventType::EventType;\
	inline static const char* EventString = #EventType         

	enum class eEventType
	{
		AppShutdownEvent,
		AppCloseEvent,
		WindowResizeEvent,
		WindowMoveEvent,

		// In Editor
		SetRunTypeEvent,
		NextFrameInSimulateEvent,
		SetEditCameraEvent,
	};

	class IEvent
	{
	public:
		virtual std::string ToString() = 0;
		virtual eEventType	GetType() = 0;

		bool m_isHandled = false;
	};

	class WindowMoveEvent : public IEvent
	{
	public:
		EVENT_IMPLEMENT(WindowMoveEvent);
		std::string ToString() override
		{
			return fmt::format("{} : ({}, {})", EventString, m_x, m_y);
		}

		uint32 m_x;
		uint32 m_y;
	};

	class WindowResizeEvent : public IEvent
	{
	public:
		EVENT_IMPLEMENT(WindowResizeEvent);
		std::string ToString() override
		{
			return fmt::format("{} : ({}, {})", EventString, m_width, m_height);
		}

		uint32 m_width;
		uint32 m_height;
	};

	class AppShutdownEvent : public IEvent
	{
	public:
		std::string ToString() { return EventString; }

		EVENT_IMPLEMENT(AppShutdownEvent);
	};

	class AppCloseEvent : public IEvent
	{
	public:
		std::string ToString() { return EventString; }

		EVENT_IMPLEMENT(AppCloseEvent);
	};

	enum class eMouse
	{
		Left = 1,
		Middle,
		Right
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

	//===================================================
	//                      EDITOR
	//===================================================


	constexpr const char* ToString(eRunType in_mode)
	{
		switch (in_mode)
		{
		case crab::eRunType::Runtime:	   return "Runtime";
		case crab::eRunType::Edit:		   return "Edit";
		case crab::eRunType::SimulatePlay: return "Simulate Play";
		case crab::eRunType::SimulateStop: return "Simulate Stop";
		default: assert(false); return "";
		}
	}

	class SetRunTypeEvent : public IEvent
	{
	public:
		std::string ToString()
		{
			return fmt::format("{} : {}", EventString, crab::ToString(m_runType));
		}

		EVENT_IMPLEMENT(SetRunTypeEvent);

		eRunType m_runType;
	};

	class NextFrameInSimulateEvent : public IEvent
	{
	public:
		std::string ToString()
		{
			return EventString;
		}

		EVENT_IMPLEMENT(NextFrameInSimulateEvent);
	};
}

