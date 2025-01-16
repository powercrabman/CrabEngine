#pragma once
#include "Enums.h"
#include "IEvent.h"

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
			return fmt::format("{} : {}", s_staticName, crab::ToString(m_editorState));
		}

		EVENT_IMPLEMENT(SetEditorState_EditorEvent);

		eEditorState m_editorState;
	};

	class NextFrame_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return s_staticName;
		}

		TimeStamp m_timeStamp;
		EVENT_IMPLEMENT(NextToFrame_EditorEvent);
	};

	class DeleteEntity_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return s_staticName;
		}

		uint32 m_entityID;

		EVENT_IMPLEMENT(DeleteEntity_EditorEvent);
	};

	class DuplicateEntity_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return s_staticName;
		}

		uint32 m_entityID;

		EVENT_IMPLEMENT(DuplicateEntity_EditorEvent);
	};

	class Scene;
	class SaveScene_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return s_staticName;
		}

		Scene* m_scene = nullptr;
		EVENT_IMPLEMENT(SaveScene_EditorEvent);
	};

	class SaveProject_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return s_staticName;
		}

		EVENT_IMPLEMENT(SaveProject_EditorEvent);
	};

	class SaveAssets_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return s_staticName;
		}

		EVENT_IMPLEMENT(SaveAssets_EditorEvent);
	};

	class RestartScene_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return s_staticName;
		}

		EVENT_IMPLEMENT(RestartScene_EditorEvent);
	};

	class SetGuizmoType_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return s_staticName;
		}

		eGuizmoType m_type;

		EVENT_IMPLEMENT(SetGuizmoType_EditorEvent);
	};

	class SendVisualLog_EditorEvent : public IEvent
	{
	public:
		std::string ToString() override
		{
			return s_staticName;
		}

		eVisualLogLevel		m_level;
		std::string_view	m_logMessage;

		EVENT_IMPLEMENT(SendVisualLog_EditorEvent);
	};
}
