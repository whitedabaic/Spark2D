#include "Keyboard.h"
#include <Logger.h>

namespace SPARK_WINDOWING::Inputs {

	Keyboard::Keyboard()
        : m_mapButtons{
        {SPARK_KEY_BACKSPACE, Button{}}, {SPARK_KEY_TAB, Button{}}, {SPARK_KEY_CLEAR, Button{}},
        {SPARK_KEY_RETURN, Button{}}, {SPARK_KEY_PAUSE, Button{}}, {SPARK_KEY_ESCAPE, Button{}}, {SPARK_KEY_SPACE, Button{}},
        {SPARK_KEY_EXCLAIM, Button{}}, {SPARK_KEY_QUOTEDBL, Button{}}, {SPARK_KEY_HASH, Button{}}, {SPARK_KEY_DOLLAR, Button{}},
        {SPARK_KEY_AMPERSAND, Button{}}, {SPARK_KEY_QUOTE, Button{}}, {SPARK_KEY_LEFTPAREN, Button{}}, {SPARK_KEY_RIGHTPAREN, Button{}},
        {SPARK_KEY_ASTERISK, Button{}}, {SPARK_KEY_PLUS, Button{}}, {SPARK_KEY_COMMA, Button{}}, {SPARK_KEY_PERIOD, Button{}},
        {SPARK_KEY_SLASH, Button{}}, {SPARK_KEY_0, Button{}}, {SPARK_KEY_2, Button{}}, {SPARK_KEY_3, Button{}},
        {SPARK_KEY_4, Button{}}, {SPARK_KEY_5, Button{}}, {SPARK_KEY_6, Button{}}, {SPARK_KEY_7, Button{}},
        {SPARK_KEY_8, Button{}}, {SPARK_KEY_9, Button{}}, {SPARK_KEY_COLON, Button{}}, {SPARK_KEY_SEMICOLON, Button{}},
        {SPARK_KEY_LESS, Button{}}, {SPARK_KEY_EQUALS, Button{}}, {SPARK_KEY_GREATER, Button{}}, {SPARK_KEY_QUESTION, Button{}},
        {SPARK_KEY_AT, Button{}}, {SPARK_KEY_LEFTBRACKET, Button{}}, {SPARK_KEY_BACKSLASH, Button{}}, {SPARK_KEY_RIGHTBRACKET, Button{}},
        {SPARK_KEY_CARET, Button{}}, {SPARK_KEY_UNDERSCORE, Button{}}, {SPARK_KEY_A, Button{}}, {SPARK_KEY_B, Button{}},
        {SPARK_KEY_C, Button{}}, {SPARK_KEY_D, Button{}}, {SPARK_KEY_E, Button{}}, {SPARK_KEY_F, Button{}},
        {SPARK_KEY_G, Button{}}, {SPARK_KEY_H, Button{}}, {SPARK_KEY_I, Button{}}, {SPARK_KEY_J, Button{}},
        {SPARK_KEY_K, Button{}}, {SPARK_KEY_L, Button{}}, {SPARK_KEY_M, Button{}}, {SPARK_KEY_N, Button{}},
        {SPARK_KEY_O, Button{}}, {SPARK_KEY_P, Button{}}, {SPARK_KEY_Q, Button{}}, {SPARK_KEY_R, Button{}},
        {SPARK_KEY_S, Button{}}, {SPARK_KEY_T, Button{}}, {SPARK_KEY_U, Button{}}, {SPARK_KEY_V, Button{}},
        {SPARK_KEY_W, Button{}}, {SPARK_KEY_X, Button{}}, {SPARK_KEY_Y, Button{}}, {SPARK_KEY_Z, Button{}},
        {SPARK_KEY_DELETE, Button{}}, {SPARK_KEY_CAPSLOCK, Button{}}, {SPARK_KEY_F1, Button{}}, {SPARK_KEY_F2, Button{}},
        {SPARK_KEY_F3, Button{}}, {SPARK_KEY_F4, Button{}}, {SPARK_KEY_F5, Button{}}, {SPARK_KEY_F6, Button{}},
        {SPARK_KEY_F7, Button{}}, {SPARK_KEY_F8, Button{}}, {SPARK_KEY_F9, Button{}}, {SPARK_KEY_F10, Button{}},
        {SPARK_KEY_F11, Button{}}, {SPARK_KEY_F12, Button{}}, {SPARK_KEY_SCROLLOCK, Button{}}, {SPARK_KEY_INSERT, Button{}},
        {SPARK_KEY_HOME, Button{}}, {SPARK_KEY_PAGEUP, Button{}}, {SPARK_KEY_PAGEDOWN, Button{}}, {SPARK_KEY_END, Button{}},
        {SPARK_KEY_RIGHT, Button{}}, {SPARK_KEY_LEFT, Button{}}, {SPARK_KEY_DOWN, Button{}}, {SPARK_KEY_UP, Button{}},
        {SPARK_KEY_NUMLOCK, Button{}}, {SPARK_KEY_KP_DIVIDE, Button{}}, {SPARK_KEY_KP_MULTIPLY, Button{}}, {SPARK_KEY_KP_MINUS, Button{}},
        {SPARK_KEY_KP_PLUS, Button{}}, {SPARK_KEY_KP_ENTER, Button{}}, {SPARK_KEY_KP1, Button{}}, {SPARK_KEY_KP2, Button{}},
        {SPARK_KEY_KP3, Button{}}, {SPARK_KEY_KP4, Button{}}, {SPARK_KEY_KP5, Button{}}, {SPARK_KEY_KP6, Button{}},
        {SPARK_KEY_KP7, Button{}}, {SPARK_KEY_KP8, Button{}}, {SPARK_KEY_KP9, Button{}}, {SPARK_KEY_KP0, Button{}},
        {SPARK_KEY_KP_PERIOD, Button{}}, {SPARK_KEY_LCTRL, Button{}}, {SPARK_KEY_LSHIFT, Button{}},
        {SPARK_KEY_LALT, Button{}}, {SPARK_KEY_RCTRL, Button{}}, {SPARK_KEY_RSHIFT, Button{}}, {SPARK_KEY_RALT, Button{}}
        }
	{
	}

	void Keyboard::Update()
	{
        for (auto& [key, button] : m_mapButtons)
            button.Reset();
	}

	void Keyboard::OnKeyPressed(int key)
	{
        if (key == KEY_UNKNOWN)
        {
            SPARK_ERROR("Key [{}] is unknown!", key);
            return;
        }
        auto keyItr = m_mapButtons.find(key);
        if (keyItr == m_mapButtons.end())
        {
            SPARK_ERROR("Key [{}] does not exist!", key);
            return;
        }
        keyItr->second.Update(true);
	}

	void Keyboard::OnKeyReleased(int key)
	{
        if (key == KEY_UNKNOWN)
        {
            SPARK_ERROR("Key [{}] is unknown!", key);
            return;
        }
        auto keyItr = m_mapButtons.find(key);
        if (keyItr == m_mapButtons.end())
        {
            SPARK_ERROR("Key [{}] does not exist!", key);
            return;
        }
        keyItr->second.Update(false);
	}

	const bool Keyboard::IsKeyPressed(int key) const
	{
        if (key == KEY_UNKNOWN)
        {
            SPARK_ERROR("Key [{}] is unknown!", key);
            return false;
        }
        auto keyItr = m_mapButtons.find(key);
        if (keyItr == m_mapButtons.end())
        {
            SPARK_ERROR("Key [{}] does not exist!", key);
            return false;
        }
        return keyItr->second.bIsPressed;
	}

	const bool Keyboard::IsKeyJustPressed(int key) const
	{
        if (key == KEY_UNKNOWN)
        {
            SPARK_ERROR("Key [{}] is unknown!", key);
            return false;
        }
        auto keyItr = m_mapButtons.find(key);
        if (keyItr == m_mapButtons.end())
        {
            SPARK_ERROR("Key [{}] does not exist!", key);
            return false;
        }
        return keyItr->second.bJustPressed;
	}

	const bool Keyboard::IsKeyJustReleased(int key) const
	{
        if (key == KEY_UNKNOWN)
        {
            SPARK_ERROR("Key [{}] is unknown!", key);
            return false;
        }
        auto keyItr = m_mapButtons.find(key);
        if (keyItr == m_mapButtons.end())
        {
            SPARK_ERROR("Key [{}] does not exist!", key);
            return false;
        }
        return keyItr->second.bJustReleased;
	}

}