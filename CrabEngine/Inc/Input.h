#pragma once
#include "Singleton.h"

namespace crab
{
	enum class eKeyCode
	{
		None = 0,

		Escape = SDL_SCANCODE_ESCAPE,
		Enter = SDL_SCANCODE_RETURN,
		Space = SDL_SCANCODE_SPACE,

		Up = SDL_SCANCODE_UP,
		Down = SDL_SCANCODE_DOWN,
		Left = SDL_SCANCODE_LEFT,
		Right = SDL_SCANCODE_RIGHT,

		W = SDL_SCANCODE_W,
		A = SDL_SCANCODE_A,
		S = SDL_SCANCODE_S,
		D = SDL_SCANCODE_D,
		Q = SDL_SCANCODE_Q,
		E = SDL_SCANCODE_E,
		R = SDL_SCANCODE_R,
		F = SDL_SCANCODE_F,
		Z = SDL_SCANCODE_Z,
		X = SDL_SCANCODE_X,
		C = SDL_SCANCODE_C
	};

	class Input
	{
	public:
		static void UpdateInputState()
		{
			std::memcpy(m_prevKeyState.data(), m_currentKeyState.data(), m_prevKeyState.size());
			const Uint8* state = SDL_GetKeyboardState(nullptr);
			std::memcpy(m_currentKeyState.data(), state, m_currentKeyState.size());
		}

		static bool IsKeyPress(eKeyCode in_keycode)
		{
			return !m_prevKeyState[static_cast<uint32>(in_keycode)] && m_currentKeyState[static_cast<uint32>(in_keycode)];
		}

		static bool IsKeyRelease(eKeyCode in_keycode)
		{
			return m_prevKeyState[static_cast<uint32>(in_keycode)] && !m_currentKeyState[static_cast<uint32>(in_keycode)];
		}

		static bool IsKeyDown(eKeyCode in_keycode)
		{
			return m_currentKeyState[static_cast<uint32>(in_keycode)];
		}

		static bool IsKeyUp(eKeyCode in_keycode)
		{
			return !m_currentKeyState[static_cast<uint32>(in_keycode)];
		}

		static auto GetMousePosition()
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			return std::make_pair(x, y);
		}

		static auto GetMousePositionDelta()
		{
			int dx, dy;
			SDL_GetRelativeMouseState(&dx, &dy);
			return std::make_pair(dx, dy);
		}

	private:
		inline static std::array<Uint8, SDL_NUM_SCANCODES> m_prevKeyState = {};
		inline static std::array<Uint8, SDL_NUM_SCANCODES> m_currentKeyState = {};
	};
}