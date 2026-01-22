#include "InputManager.h"
#include <Logger/Logger.h>
#include <SparkUtilities/SDL_Wrappers.h>

namespace SPARK_CORE {
	InputManager::InputManager()
        : m_pKeyboard{std::make_unique<Keyboard>()}, m_pMouse{ std::make_unique<Mouse>() }
	{

	}

	void InputManager::ReisterLuaKeyNames(sol::state& lua)
	{
        // ==================================================================
      // Register Typewriter Keys
      // ==================================================================
        lua.set("KEY_A", SPARK_KEY_A);
        lua.set("KEY_B", SPARK_KEY_B);
        lua.set("KEY_C", SPARK_KEY_C);
        lua.set("KEY_D", SPARK_KEY_D);
        lua.set("KEY_E", SPARK_KEY_E);
        lua.set("KEY_F", SPARK_KEY_F);
        lua.set("KEY_G", SPARK_KEY_G);
        lua.set("KEY_H", SPARK_KEY_H);
        lua.set("KEY_I", SPARK_KEY_I);
        lua.set("KEY_J", SPARK_KEY_J);
        lua.set("KEY_K", SPARK_KEY_K);
        lua.set("KEY_L", SPARK_KEY_L);
        lua.set("KEY_M", SPARK_KEY_M);
        lua.set("KEY_N", SPARK_KEY_N);
        lua.set("KEY_O", SPARK_KEY_O);
        lua.set("KEY_P", SPARK_KEY_P);
        lua.set("KEY_Q", SPARK_KEY_Q);
        lua.set("KEY_R", SPARK_KEY_R);
        lua.set("KEY_S", SPARK_KEY_S);
        lua.set("KEY_T", SPARK_KEY_T);
        lua.set("KEY_U", SPARK_KEY_U);
        lua.set("KEY_V", SPARK_KEY_V);
        lua.set("KEY_W", SPARK_KEY_W);
        lua.set("KEY_X", SPARK_KEY_X);
        lua.set("KEY_Y", SPARK_KEY_Y);
        lua.set("KEY_Z", SPARK_KEY_Z);

        lua.set("KEY_0", SPARK_KEY_0);
        lua.set("KEY_1", SPARK_KEY_1);
        lua.set("KEY_2", SPARK_KEY_2);
        lua.set("KEY_3", SPARK_KEY_3);
        lua.set("KEY_4", SPARK_KEY_4);
        lua.set("KEY_5", SPARK_KEY_5);
        lua.set("KEY_6", SPARK_KEY_6);
        lua.set("KEY_7", SPARK_KEY_7);
        lua.set("KEY_8", SPARK_KEY_8);
        lua.set("KEY_9", SPARK_KEY_9);

        lua.set("KEY_ENTER", SPARK_KEY_RETURN);
        lua.set("KEY_BACKSPACE", SPARK_KEY_BACKSPACE);
        lua.set("KEY_ESC", SPARK_KEY_ESCAPE);
        lua.set("KEY_SPACE", SPARK_KEY_SPACE);
        lua.set("KEY_LCTRL", SPARK_KEY_LCTRL);
        lua.set("KEY_RCTRL", SPARK_KEY_RCTRL);
        lua.set("KEY_LALT", SPARK_KEY_LALT);
        lua.set("KEY_RALT", SPARK_KEY_RALT);
        lua.set("KEY_LSHIFT", SPARK_KEY_LSHIFT);
        lua.set("KEY_RSHIFT", SPARK_KEY_RSHIFT);

        // ==================================================================
        //  Register Function Keys
        // ==================================================================
        lua.set("KEY_F1", SPARK_KEY_F1);
        lua.set("KEY_F2", SPARK_KEY_F2);
        lua.set("KEY_F3", SPARK_KEY_F3);
        lua.set("KEY_F4", SPARK_KEY_F4);
        lua.set("KEY_F5", SPARK_KEY_F5);
        lua.set("KEY_F6", SPARK_KEY_F6);
        lua.set("KEY_F7", SPARK_KEY_F7);
        lua.set("KEY_F8", SPARK_KEY_F8);
        lua.set("KEY_F9", SPARK_KEY_F9);
        lua.set("KEY_F10", SPARK_KEY_F10);
        lua.set("KEY_F11", SPARK_KEY_F11);
        lua.set("KEY_F12", SPARK_KEY_F12);

        // ==================================================================
        // Register Cursor Control Keys
        // ==================================================================
        lua.set("KEY_UP", SPARK_KEY_UP);
        lua.set("KEY_RIGHT", SPARK_KEY_RIGHT);
        lua.set("KEY_DOWN", SPARK_KEY_DOWN);
        lua.set("KEY_LEFT", SPARK_KEY_LEFT);

        // ==================================================================
        // Register Numeric Keypad Keys
        // ==================================================================
        lua.set("KP_KEY_0", SPARK_KEY_KP0);
        lua.set("KP_KEY_1", SPARK_KEY_KP1);
        lua.set("KP_KEY_2", SPARK_KEY_KP2);
        lua.set("KP_KEY_3", SPARK_KEY_KP3);
        lua.set("KP_KEY_4", SPARK_KEY_KP4);
        lua.set("KP_KEY_5", SPARK_KEY_KP5);
        lua.set("KP_KEY_6", SPARK_KEY_KP6);
        lua.set("KP_KEY_7", SPARK_KEY_KP7);
        lua.set("KP_KEY_8", SPARK_KEY_KP8);
        lua.set("KP_KEY_9", SPARK_KEY_KP9);
        lua.set("KP_KEY_ENTER", SPARK_KEY_KP_ENTER);
	}

    void InputManager::RegisterMouseBtnNames(sol::state& lua)
    {
        lua.set("LEFT_BTN", SPARK_MOUSE_LEFT);
        lua.set("MIDDLE_BTN", SPARK_MOUSE_MIDDLE);
        lua.set("RIGHT_BTN", SPARK_MOUSE_RIGHT);
    }

    void InputManager::RegisterGamepadBtnNames(sol::state& lua)
    {
        lua.set("GP_BTN_A", SPARK_GP_BTN_A);
        lua.set("GP_BTN_B", SPARK_GP_BTN_B);
        lua.set("GP_BTN_X", SPARK_GP_BTN_X);
        lua.set("GP_BTN_Y", SPARK_GP_BTN_Y);

        lua.set("GP_BTN_BACK", SPARK_GP_BTN_BACK);
        lua.set("GP_BTN_GUIDE", SPARK_GP_BTN_GUIDE);
        lua.set("GP_BTN_START", SPARK_GP_BTN_START);

        lua.set("GP_LSTICK", SPARK_GP_BTN_LSTICK);
        lua.set("GP_RSTICK", SPARK_GP_BTN_RSTICK);
        lua.set("GP_LSHOULDER", SPARK_GP_BTN_LSHOULDER);
        lua.set("GP_RSHOULDER", SPARK_GP_BTN_RSHOULDER);

        lua.set("DPAD_UP", SPARK_GP_BTN_DPAD_UP);
        lua.set("DPAD_DOWN", SPARK_GP_BTN_DPAD_DOWN);
        lua.set("DPAD_LEFT", SPARK_GP_BTN_DPAD_LEFT);
        lua.set("DPAD_RIGHT", SPARK_GP_BTN_DPAD_RIGHT);

        lua.set("AXIS_X1", 0); lua.set("AXIS_Y1", 1);
        lua.set("AXIS_X2", 2); lua.set("AXIS_Y2", 3);

        // Bottom triggers
        lua.set("AXIS_Z1", 4); lua.set("AXIS_Z2", 5);
    }

	InputManager& InputManager::GetInstance()
	{
		static InputManager instance{};
		return instance;
	}

	void InputManager::CreateLuaInputBindings(sol::state& lua)
	{
        ReisterLuaKeyNames(lua);
        RegisterMouseBtnNames(lua);
        RegisterGamepadBtnNames(lua);

        auto& inputManager = GetInstance();
        auto& keyboard = inputManager.GetKeyboard();

        lua.new_usertype<Keyboard>(
            "Keyboard",
            sol::no_constructor,
            "just_pressed", [&](int key) { return keyboard.IsKeyJustPressed(key); },
            "just_released", [&](int key) { return keyboard.IsKeyJustReleased(key); },
            "pressed", [&](int key) { return keyboard.IsKeyPressed(key); }
        );

        auto& mouse = inputManager.GetMouse();

        lua.new_usertype<Mouse>(
            "Mouse",
            sol::no_constructor,
            "just_pressed", [&](int btn) { return mouse.IsBtnJustPressed(btn); },
            "just_released", [&](int btn) { return mouse.IsBtnJustReleased(btn); },
            "pressed", [&](int btn) { return mouse.IsBtnPressed(btn); },
            "screen_position", [&]() { return mouse.GetMouseScreenPosition(); },
            "wheel_x", [&]() { return mouse.GetMouseWheelX(); },
            "wheel_y", [&]() { return mouse.GetMouseWheelY(); }
        );

        lua.new_usertype<Gamepad>(
            "Gamepad",
            sol::no_constructor,
            "just_pressed", [&](int index, int btn) {
                auto gamepad = inputManager.GetController(index);
                if (!gamepad)
                {
                    SPARK_ERROR("Invalid Gamepad index [{}] provided or gamepad is not plugged in!", index);
                    return false;
                }
                return gamepad->IsBtnJustPressed(btn);
            },
            "just_released", [&](int index, int btn) {
                auto gamepad = inputManager.GetController(index);
                if (!gamepad)
                {
                    SPARK_ERROR("Invalid Gamepad index [{}] provided or gamepad is not plugged in!", index);
                    return false;
                }
                return gamepad->IsBtnJustReleased(btn);
            },
            "pressed", [&](int index, int btn) {
                auto gamepad = inputManager.GetController(index);
                if (!gamepad)
                {
                    SPARK_ERROR("Invalid Gamepad index [{}] provided or gamepad is not plugged in!", index);
                    return false;
                }
                return gamepad->IsBtnPressed(btn);
            },
            "get_axis_position", [&](int index, int axis) {
                auto gamepad = inputManager.GetController(index);
                if (!gamepad)
                {
                    SPARK_ERROR("Invalid Gamepad index [{}] provided or gamepad is not plugged in!", index);
                    return Sint16{ 0 };
                }
                return gamepad->GetAxisPosition(axis);
            },
            "get_hat_value", [&](int index) {
                auto gamepad = inputManager.GetController(index);
                if (!gamepad)
                {
                    SPARK_ERROR("Invalid Gamepad index [{}] provided or gamepad is not plugged in!", index);
                    return Uint8{ 0 };
                }
                return gamepad->GetJoystickHatValue();
            }
        );
	}

    std::shared_ptr<Gamepad> InputManager::GetController(int index)
    {
        auto gamepadItr = m_mapGameControllers.find(index);
        if (gamepadItr == m_mapGameControllers.end())
        {
            SPARK_ERROR("Failed to get gamepad at index [{}] -- Does not exist!", index);
            return nullptr;
        }

        return gamepadItr->second;
    }

    bool InputManager::AddGamepad(Sint32 gamepadIndex)
    {
        if (m_mapGameControllers.size() >= MAX_CONTROLLERS)
        {
            SPARK_ERROR("Trying to add too many controllers! Max Controllers allowed = {}", MAX_CONTROLLERS);
            return false;
        }

        std::shared_ptr<Gamepad> gamepad{ nullptr };
        try 
        {
            gamepad = std::make_shared<Gamepad>(
                std::move(make_shared_controller(SDL_GameControllerOpen(gamepadIndex))));
        }
        catch (...)
        {
            std::string error{ SDL_GetError() };
            SPARK_ERROR("Failed to Open gamepad device -- {}", error);
            return false;
        }

        for (int i = 1; i <= MAX_CONTROLLERS; i++)
        {
            if (m_mapGameControllers.contains(i))
                continue;

            m_mapGameControllers.emplace(i, std::move(gamepad));
            SPARK_LOG("Gamepad [{}] was added at index [{}]", gamepadIndex, i);
            return true;
        }

        SPARK_ASSERT(false && "Failed to add the new controller!");
        SPARK_ERROR("Failed to add the new controller!");
        return false;
    }

    bool InputManager::RemoveGamepad(Sint32 gamepadID)
    {
        auto gamepadRemoved = std::erase_if(m_mapGameControllers,
            [&](auto& gamepad) {
                return gamepad.second->CheckJoystickID(gamepadID);
            }
        );

        if (gamepadRemoved > 0)
        {
            SPARK_LOG("Gamepad Removed -- [{}]", gamepadID);
            return true;
        }

        SPARK_ASSERT(false && "Failed to remove Gamepad must not have been mapped!");
        SPARK_ERROR("Failed to remove Gamepad [{}] must not have been mapped!", gamepadID);
        return false;
    }

    void InputManager::GamepadBtnPressed(const SDL_Event& event)
    {
        for (const auto& [index, gamepad] : m_mapGameControllers)
        {
            if (gamepad && gamepad->CheckJoystickID(event.jdevice.which))
            {
                gamepad->OnBtnPressed(event.cbutton.button);
                break;
            }
        }
    }

    void InputManager::GamepadBtnReleased(const SDL_Event& event)
    {
        for (const auto& [index, gamepad] : m_mapGameControllers)
        {
            if (gamepad && gamepad->CheckJoystickID(event.jdevice.which))
            {
                gamepad->OnBtnReleased(event.cbutton.button);
                break;
            }
        }
    }

    void InputManager::GamepadAxisValues(const SDL_Event& event)
    {
        for (const auto& [index, gamepad] : m_mapGameControllers)
        {
            if (gamepad && gamepad->CheckJoystickID(event.jdevice.which))
            {
                gamepad->SetAxisPositionValue(event.jaxis.axis, event.jaxis.value);
                break;
            }
        }
    }

    void InputManager::GamepadHatValues(const SDL_Event& event)
    {
        for (const auto& [index, gamepad] : m_mapGameControllers)
        {
            if (gamepad && gamepad->CheckJoystickID(event.jdevice.which))
            {
                gamepad->SetJoystickHatValue(event.jhat.value);
                break;
            }
        }
    }

    void InputManager::UpdateGamepads()
    {
        for (const auto& [index, gamepad] : m_mapGameControllers)
        {
            if (gamepad)
                gamepad->Update();
        }
    }
}