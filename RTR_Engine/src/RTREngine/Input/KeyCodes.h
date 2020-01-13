#pragma once

namespace RTREngine
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39,	/* ' */
		Comma = 44,			/* , */
		Minus = 45,			/* - */
		Period = 46,		/* . */
		Slash = 47,			/* / */

		D0 = 48,	/* 0 */
		D1 = 49,	/* 1 */
		D2 = 50,	/* 2 */
		D3 = 51,	/* 3 */
		D4 = 52,	/* 4 */
		D5 = 53,	/* 5 */
		D6 = 54,	/* 6 */
		D7 = 55,	/* 7 */
		D8 = 56,	/* 8 */
		D9 = 57,	/* 9 */

		Semicolon = 59, /* ; */
		Equal = 61,		/* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,	/* [ */
		Backslash = 92,		/* \ */
		RightBracket = 93,	/* ] */
		GraveAccent = 96,	/* ` */

		World1 = 161,	/* non-US #1 */
		World2 = 162,	/* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define RTR_KEY_SPACE           ::RTREngine::Key::Space
#define RTR_KEY_APOSTROPHE      ::RTREngine::Key::Apostrophe    /* ' */
#define RTR_KEY_COMMA           ::RTREngine::Key::Comma         /* , */
#define RTR_KEY_MINUS           ::RTREngine::Key::Minus         /* - */
#define RTR_KEY_PERIOD          ::RTREngine::Key::Period        /* . */
#define RTR_KEY_SLASH           ::RTREngine::Key::Slash         /* / */
#define RTR_KEY_0               ::RTREngine::Key::D0
#define RTR_KEY_1               ::RTREngine::Key::D1
#define RTR_KEY_2               ::RTREngine::Key::D2
#define RTR_KEY_3               ::RTREngine::Key::D3
#define RTR_KEY_4               ::RTREngine::Key::D4
#define RTR_KEY_5               ::RTREngine::Key::D5
#define RTR_KEY_6               ::RTREngine::Key::D6
#define RTR_KEY_7               ::RTREngine::Key::D7
#define RTR_KEY_8               ::RTREngine::Key::D8
#define RTR_KEY_9               ::RTREngine::Key::D9
#define RTR_KEY_SEMICOLON       ::RTREngine::Key::Semicolon     /* ; */
#define RTR_KEY_EQUAL           ::RTREngine::Key::Equal         /* = */
#define RTR_KEY_A               ::RTREngine::Key::A
#define RTR_KEY_B               ::RTREngine::Key::B
#define RTR_KEY_C               ::RTREngine::Key::C
#define RTR_KEY_D               ::RTREngine::Key::D
#define RTR_KEY_E               ::RTREngine::Key::E
#define RTR_KEY_F               ::RTREngine::Key::F
#define RTR_KEY_G               ::RTREngine::Key::G
#define RTR_KEY_H               ::RTREngine::Key::H
#define RTR_KEY_I               ::RTREngine::Key::I
#define RTR_KEY_J               ::RTREngine::Key::J
#define RTR_KEY_K               ::RTREngine::Key::K
#define RTR_KEY_L               ::RTREngine::Key::L
#define RTR_KEY_M               ::RTREngine::Key::M
#define RTR_KEY_N               ::RTREngine::Key::N
#define RTR_KEY_O               ::RTREngine::Key::O
#define RTR_KEY_P               ::RTREngine::Key::P
#define RTR_KEY_Q               ::RTREngine::Key::Q
#define RTR_KEY_R               ::RTREngine::Key::R
#define RTR_KEY_S               ::RTREngine::Key::S
#define RTR_KEY_T               ::RTREngine::Key::T
#define RTR_KEY_U               ::RTREngine::Key::U
#define RTR_KEY_V               ::RTREngine::Key::V
#define RTR_KEY_W               ::RTREngine::Key::W
#define RTR_KEY_X               ::RTREngine::Key::X
#define RTR_KEY_Y               ::RTREngine::Key::Y
#define RTR_KEY_Z               ::RTREngine::Key::Z
#define RTR_KEY_LEFT_BRACKET    ::RTREngine::Key::LeftBracket   /* [ */
#define RTR_KEY_BACKSLASH       ::RTREngine::Key::Backslash     /* \ */
#define RTR_KEY_RIGHT_BRACKET   ::RTREngine::Key::RightBracket  /* ] */
#define RTR_KEY_GRAVE_ACCENT    ::RTREngine::Key::GraveAccent   /* ` */
#define RTR_KEY_WORLD_1         ::RTREngine::Key::World1        /* non-US #1 */
#define RTR_KEY_WORLD_2         ::RTREngine::Key::World2        /* non-US #2 */

/* Function keys */
#define RTR_KEY_ESCAPE          ::RTREngine::Key::Escape
#define RTR_KEY_ENTER           ::RTREngine::Key::Enter
#define RTR_KEY_TAB             ::RTREngine::Key::Tab
#define RTR_KEY_BACKSPACE       ::RTREngine::Key::Backspace
#define RTR_KEY_INSERT          ::RTREngine::Key::Insert
#define RTR_KEY_DELETE          ::RTREngine::Key::Delete
#define RTR_KEY_RIGHT           ::RTREngine::Key::Right
#define RTR_KEY_LEFT            ::RTREngine::Key::Left
#define RTR_KEY_DOWN            ::RTREngine::Key::Down
#define RTR_KEY_UP              ::RTREngine::Key::Up
#define RTR_KEY_PAGE_UP         ::RTREngine::Key::PageUp
#define RTR_KEY_PAGE_DOWN       ::RTREngine::Key::PageDown
#define RTR_KEY_HOME            ::RTREngine::Key::Home
#define RTR_KEY_END             ::RTREngine::Key::End
#define RTR_KEY_CAPS_LOCK       ::RTREngine::Key::CapsLock
#define RTR_KEY_SCROLL_LOCK     ::RTREngine::Key::ScrollLock
#define RTR_KEY_NUM_LOCK        ::RTREngine::Key::NumLock
#define RTR_KEY_PRINT_SCREEN    ::RTREngine::Key::PrintScreen
#define RTR_KEY_PAUSE           ::RTREngine::Key::Pause
#define RTR_KEY_F1              ::RTREngine::Key::F1
#define RTR_KEY_F2              ::RTREngine::Key::F2
#define RTR_KEY_F3              ::RTREngine::Key::F3
#define RTR_KEY_F4              ::RTREngine::Key::F4
#define RTR_KEY_F5              ::RTREngine::Key::F5
#define RTR_KEY_F6              ::RTREngine::Key::F6
#define RTR_KEY_F7              ::RTREngine::Key::F7
#define RTR_KEY_F8              ::RTREngine::Key::F8
#define RTR_KEY_F9              ::RTREngine::Key::F9
#define RTR_KEY_F10             ::RTREngine::Key::F10
#define RTR_KEY_F11             ::RTREngine::Key::F11
#define RTR_KEY_F12             ::RTREngine::Key::F12
#define RTR_KEY_F13             ::RTREngine::Key::F13
#define RTR_KEY_F14             ::RTREngine::Key::F14
#define RTR_KEY_F15             ::RTREngine::Key::F15
#define RTR_KEY_F16             ::RTREngine::Key::F16
#define RTR_KEY_F17             ::RTREngine::Key::F17
#define RTR_KEY_F18             ::RTREngine::Key::F18
#define RTR_KEY_F19             ::RTREngine::Key::F19
#define RTR_KEY_F20             ::RTREngine::Key::F20
#define RTR_KEY_F21             ::RTREngine::Key::F21
#define RTR_KEY_F22             ::RTREngine::Key::F22
#define RTR_KEY_F23             ::RTREngine::Key::F23
#define RTR_KEY_F24             ::RTREngine::Key::F24
#define RTR_KEY_F25             ::RTREngine::Key::F25

/* Keypad */
#define RTR_KEY_KP_0            ::RTREngine::Key::KP0
#define RTR_KEY_KP_1            ::RTREngine::Key::KP1
#define RTR_KEY_KP_2            ::RTREngine::Key::KP2
#define RTR_KEY_KP_3            ::RTREngine::Key::KP3
#define RTR_KEY_KP_4            ::RTREngine::Key::KP4
#define RTR_KEY_KP_5            ::RTREngine::Key::KP5
#define RTR_KEY_KP_6            ::RTREngine::Key::KP6
#define RTR_KEY_KP_7            ::RTREngine::Key::KP7
#define RTR_KEY_KP_8            ::RTREngine::Key::KP8
#define RTR_KEY_KP_9            ::RTREngine::Key::KP9
#define RTR_KEY_KP_DECIMAL      ::RTREngine::Key::KPDecimal
#define RTR_KEY_KP_DIVIDE       ::RTREngine::Key::KPDivide
#define RTR_KEY_KP_MULTIPLY     ::RTREngine::Key::KPMultiply
#define RTR_KEY_KP_SUBTRACT     ::RTREngine::Key::KPSubtract
#define RTR_KEY_KP_ADD          ::RTREngine::Key::KPAdd
#define RTR_KEY_KP_ENTER        ::RTREngine::Key::KPEnter
#define RTR_KEY_KP_EQUAL        ::RTREngine::Key::KPEqual

#define RTR_KEY_LEFT_SHIFT      ::RTREngine::Key::LeftShift
#define RTR_KEY_LEFT_CONTROL    ::RTREngine::Key::LeftControl
#define RTR_KEY_LEFT_ALT        ::RTREngine::Key::LeftAlt
#define RTR_KEY_LEFT_SUPER      ::RTREngine::Key::LeftSuper
#define RTR_KEY_RIGHT_SHIFT     ::RTREngine::Key::RightShift
#define RTR_KEY_RIGHT_CONTROL   ::RTREngine::Key::RightControl
#define RTR_KEY_RIGHT_ALT       ::RTREngine::Key::RightAlt
#define RTR_KEY_RIGHT_SUPER     ::RTREngine::Key::RightSuper
#define RTR_KEY_MENU            ::RTREngine::Key::Menu