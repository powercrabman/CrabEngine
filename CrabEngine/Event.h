#pragma once

namespace crab
{
#define EVENT_IMPLEMENT(EventType) \
	const char* ToString() override {return #EventType;}\
	eEventType	GetType() override {return eEventType::EventType;}\
	inline static eEventType StaticType = eEventType::EventType

	enum class eEventType
	{
		AppShutdownEvent,
		AppCloseEvent,
		WindowResizeEvent,
	};

	class IEvent
	{
	public:
		virtual const char* ToString() = 0;
		virtual eEventType	GetType() = 0;

		bool m_isHandled = false;
	};

	class WindowResizeEvent : public IEvent
	{
	public:
		EVENT_IMPLEMENT(WindowResizeEvent);

		uint32 m_width;
		uint32 m_height;
	};

	class AppShutdownEvent : public IEvent
	{
	public:
		EVENT_IMPLEMENT(AppShutdownEvent);
	};

	class AppCloseEvent : public IEvent
	{
	public:
		EVENT_IMPLEMENT(AppCloseEvent);
	};
}

