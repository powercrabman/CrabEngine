#pragma once
#include "Enums.h"
#include "Event.h"

namespace crab
{
	constexpr const char* ToString(eEditorState in_mode)
	{
		switch (in_mode)
		{
		case crab::eEditorState::Runtime:	   return "Runtime";
		case crab::eEditorState::Edit:		   return "Edit";
		case crab::eEditorState::SimulatePlay: return "Simulate Play";
		case crab::eEditorState::SimulateStop: return "Simulate Stop";
		default: assert(false); return "";
		}
	}

	class SetEditorState_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return fmt::format("{} : {}", EventString, crab::ToString(m_editorState));
		}

		EVENT_IMPLEMENT(SetEditorState_EditorEvent);

		eEditorState m_editorState;
	};

	class NextFrame_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return EventString;
		}

		TimeStamp m_timeStamp;
		EVENT_IMPLEMENT(NextToFrame_EditorEvent);
	};

	class DeleteEntity_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return EventString;
		}

		uint32 m_entityID;

		EVENT_IMPLEMENT(DeleteEntity_EditorEvent);
	};

	class DuplicateEntity_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return EventString;
		}

		uint32 m_entityID;

		EVENT_IMPLEMENT(DuplicateEntity_EditorEvent);
	};

	class SaveScene_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return EventString;
		}

		EVENT_IMPLEMENT(SaveScene_EditorEvent);
	};

	class ReloadScene_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return EventString;
		}

		EVENT_IMPLEMENT(ReloadScene_EditorEvent);
	};

	class SetGuizmoType_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return EventString;
		}

		eGuizmoType m_type;

		EVENT_IMPLEMENT(SetGuizmoType_EditorEvent);
	};
}
