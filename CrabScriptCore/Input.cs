using System;
using System.Runtime.CompilerServices;


namespace crab
{
	public enum eKey
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

	public enum eMouse
	{ 
		Left = 0,
		Right,
		Middle
	}

	public static class Input
	{
		public static bool IsKeyPress(eKey in_keyCode) { return InternalCalls.Input_IsKeyPress(in_keyCode); }
		public static bool IsKeyRelease(eKey in_keyCode) { return InternalCalls.Input_IsKeyRelease(in_keyCode); }
		public static bool IsKeyDown(eKey in_keyCode) { return InternalCalls.Input_IsKeyDown(in_keyCode); }
		public static bool IsKeyUp(eKey in_keyCode) { return InternalCalls.Input_IsKeyUp(in_keyCode); }
	}
}
