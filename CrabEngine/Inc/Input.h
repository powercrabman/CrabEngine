#pragma once
#include "Singleton.h"

namespace crab
{
	enum class eKey
	{
		None = 0,
		NamedKey_BEGIN = 512, 

		Tab = 512,
		LeftArrow,
		RightArrow,
		UpArrow,
		DownArrow,
		PageUp,
		PageDown,
		Home,
		End,
		Insert,
		Delete,
		Backspace,
		Space,
		Enter,
		Escape,
		LeftCtrl,
		LeftShift, 
		LeftAlt, LeftSuper,
		RightCtrl, RightShift, RightAlt, RightSuper,
		Menu,
		Num_0, Num_1, Num_2, Num_3, Num_4, Num_5, Num_6, Num_7, Num_8, Num_9,
		A, B, C, D, E, F, G, H, I, J,
		K, L, M, N, O, P, Q, R, S, T,
		U, V, W, X, Y, Z,
		F1, F2, F3, F4, F5, F6,
		F7, F8, F9, F10, F11, F12,
		F13, F14, F15, F16, F17, F18,
		F19, F20, F21, F22, F23, F24,
		Apostrophe,       
		Comma,            
		Minus,            
		Period,           
		Slash,            
		Semicolon,        
		Equal,            
		LeftBracket,      
		Backslash,        
		RightBracket,     
		GraveAccent,      
		CapsLock,
		ScrollLock,
		NumLock,
		PrintScreen,
		Pause,
		Keypad0, Keypad1, Keypad2, Keypad3, Keypad4,
		Keypad5, Keypad6, Keypad7, Keypad8, Keypad9,
		KeypadDecimal,
		KeypadDivide,
		KeypadMultiply,
		KeypadSubtract,
		KeypadAdd,
		KeypadEnter,
		KeypadEqual,
		AppBack,           
		AppForward,
	};

	class Input
	{
	public:
		// TODO - 임시적으로 ImGui의 Io를 뺏어서 사용하고 있음
		static void UpdateInputState()
		{
			ImGuiIO& io = ImGui::GetIO();
			m_state.mouseWheelX = io.MouseWheelH;
			m_state.mouseWheelY = io.MouseWheel;
		}
		
		static bool IsKeyPress(eKey in_keycode, bool repeat = false)
		{
			return ImGui::IsKeyPressed((ImGuiKey)in_keycode, repeat);
		}

		static bool IsKeyRelease(eKey in_keycode)
		{
			return ImGui::IsKeyReleased((ImGuiKey)in_keycode);
		}

		static bool IsKeyDown(eKey in_keycode)
		{
			return ImGui::IsKeyDown((ImGuiKey)in_keycode);
		}

		static bool IsKeyUp(eKey in_keycode)
		{
			return !ImGui::IsKeyDown((ImGuiKey)in_keycode);
		}

		static bool IsMousePress(eMouse in_mouse)
		{
			return ImGui::IsMouseClicked((uint32)in_mouse);
		}

		static bool IsMouseRelease(eMouse in_mouse)
		{
			return ImGui::IsMouseReleased((uint32)in_mouse);
		}

		static bool IsMouseDown(eMouse in_mouse)
		{
			return ImGui::IsMouseDown((uint32)in_mouse);
		}

		static bool IsMouseUp(eMouse in_mouse)
		{
			return !ImGui::IsMouseDown((uint32)in_mouse);
		}

		static auto GetMousePos()
		{
			ImVec2 pos = ImGui::GetMousePos();
			return std::make_pair(pos.x, pos.y);
		}

		static auto GetMousePosDelta()
		{
			ImVec2 dt = ImGui::GetIO().MouseDelta;
			return std::make_pair(dt.x, dt.y);
		}

		static float GetMouseScrollDeltaY()
		{
			return m_state.mouseWheelY;
		}

		static float GetMouseScrollDeltaX()
		{
			return m_state.mouseWheelX;
		}

	private:
		struct InputState
		{
			float mouseWheelX = 0.f;
			float mouseWheelY = 0.f;
		};

		inline static InputState m_state;
	};
}


//===================================================
//                     LAGACY
//===================================================

/*

#pragma once
#include "Singleton.h"

namespace crab
{
	//enum class eKeyCode
	//{
	//	None = 0,
	//
	//	Escape = SDL_SCANCODE_ESCAPE,
	//	Enter = SDL_SCANCODE_RETURN,
	//	Space = SDL_SCANCODE_SPACE,
	//
	//	Up = SDL_SCANCODE_UP,
	//	Down = SDL_SCANCODE_DOWN,
	//	Left = SDL_SCANCODE_LEFT,
	//	Right = SDL_SCANCODE_RIGHT,
	//
	//	W = SDL_SCANCODE_W,
	//	A = SDL_SCANCODE_A,
	//	S = SDL_SCANCODE_S,
	//	D = SDL_SCANCODE_D,
	//	Q = SDL_SCANCODE_Q,
	//	E = SDL_SCANCODE_E,
	//	R = SDL_SCANCODE_R,
	//	F = SDL_SCANCODE_F,
	//	Z = SDL_SCANCODE_Z,
	//	X = SDL_SCANCODE_X,
	//	C = SDL_SCANCODE_C
	//};

	enum class eKeyCode
	{

	};

	class Input
	{
	public:
		static void UpdateInputState()
		{
			//std::memcpy(m_prevKeyState.data(), m_currentKeyState.data(), m_prevKeyState.size());
			//const Uint8* state = SDL_GetKeyboardState(nullptr);
			//std::memcpy(m_currentKeyState.data(), state, m_currentKeyState.size());
		}

		static bool IsKeyPress(eKeyCode in_keycode)
		{
			//return !m_prevKeyState[static_cast<UInt32>(in_keycode)] && m_currentKeyState[static_cast<UInt32>(in_keycode)];
			return ImGui::IsKeyPressed((ImGuiKey)in_keycode);
		}

		static bool IsKeyRelease(eKeyCode in_keycode)
		{
			//return m_prevKeyState[static_cast<UInt32>(in_keycode)] && !m_currentKeyState[static_cast<UInt32>(in_keycode)];
			return ImGui::IsKeyReleased((ImGuiKey)in_keycode);
		}

		static bool IsKeyDown(eKeyCode in_keycode)
		{
			//return m_currentKeyState[static_cast<UInt32>(in_keycode)];
			return ImGui::IsKeyDown((ImGuiKey)in_keycode);
		}

		static bool IsKeyUp(eKeyCode in_keycode)
		{
			//return !m_currentKeyState[static_cast<UInt32>(in_keycode)];
			return !ImGui::IsKeyDown((ImGuiKey)in_keycode);
		}

		static bool IsMouseDown(eMouse in_mouse)
		{
			//return ImGui::IsMouseDown((UInt32)in_mouse - 1);
			return ImGui::IsMouseDown((UInt32)in_mouse - 1);
		}

		static bool IsMouseUp(eMouse in_mouse)
		{
			//return ImGui::IsMouseUp((UInt32)in_mouse - 1);
			return !ImGui::IsMouseDown((UInt32)in_mouse - 1);
		}

		static auto GetMousePosition()
		{
			//int x, y;
			//SDL_GetMouseState(&x, &y);
			//return std::make_pair(x, y);
			auto [x, y] = ImGui::GetMousePos();
			return std::make_pair(x, y);
		}

		static auto GetMousePositionDelta(eMouse in_mouse)
		{
			//int dx, dy;
			//SDL_GetRelativeMouseState(&dx, &dy);
			//return std::make_pair(dx, dy);
			auto [dx, dy] = ImGui::GetMouseDragDelta((UInt32)in_mouse - 1);
			return std::make_pair(dx, dy);
		}

		static float GetMouseScrollDelta()
		{
			return ImGui::GetIO().MouseWheel;
		}

	private:
		//inline static std::array<Uint8, SDL_NUM_SCANCODES> m_prevKeyState = {};
		//inline static std::array<Uint8, SDL_NUM_SCANCODES> m_currentKeyState = {};
	};
}

*/