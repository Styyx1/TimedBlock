#include "HotkeyManager.h"
#include "Utility.h"
#include "PCH.h"

namespace Events {
    HotkeyManager* HotkeyManager::GetSingleton()
    {
        static HotkeyManager singleton;
        return &singleton;
    }

    void HotkeyManager::Register()
    {
        if (const auto scripts = RE::BSInputDeviceManager::GetSingleton()) {
            scripts->AddEventSink<RE::InputEvent*>(GetSingleton());
            logger::info("Registered for {}", typeid(RE::InputEvent).name());
        }
    }

    void HotkeyManager::TimedBlockKey(const hotkeys::KeyCombination* key)
    {
        Utility* util = Utility::GetSingleton();
        Settings* set = Settings::GetSingleton();
        RE::PlayerCharacter* p = Cache::GetPlayerSingleton();
        if (!util->PlayerHasActiveMagicEffect(set->mgef_parry_window)) {
            util->ApplySpell(p, p, set->spell_parry_window);
            logger::debug("applied {} to {}", set->spell_parry_window->GetName(), p->GetDisplayFullName());
        }        
        return;
    }

    void HotkeyManager::SetBlockKey()
    {
        const RE::ControlMap* cm        = RE::ControlMap::GetSingleton();
        const RE::UserEvents* userEvent = RE::UserEvents::GetSingleton();

        for (int i = RE::INPUT_DEVICE::kKeyboard; i <= RE::INPUT_DEVICE::kGamepad; ++i) {
            switch (i) {
            case RE::INPUT_DEVICE::kKeyboard:
                if (cm->GetMappedKey(userEvent->leftAttack, RE::INPUT_DEVICE::kKeyboard) != 0) {
                    logger::debug("no keyboard key mapped");
                    break;
                }
                else {
                    blockKey.SetPattern(hotkeys::details::GetNameByKey(cm->GetMappedKey(userEvent->leftAttack, RE::INPUT_DEVICE::kKeyboard)));
                    logger::debug("KeyCode for keyboard block is {} \n", blockKey.GetPattern());
                    break;
                }
                
            case RE::INPUT_DEVICE::kMouse:
                blockKeyMouse.SetPattern(hotkeys::details::GetNameByKey(SKSE::InputMap::kMacro_MouseButtonOffset + cm->GetMappedKey(userEvent->leftAttack, RE::INPUT_DEVICE::kMouse)));
                logger::debug("KeyCode for mouse block is {} \n ", blockKeyMouse.GetPattern());
                break;
            case RE::INPUT_DEVICE::kGamepad:
                blockKeyController.SetPattern(hotkeys::details::GetNameByKey(SKSE::InputMap::GamepadMaskToKeycode(cm->GetMappedKey(userEvent->leftAttack, RE::INPUT_DEVICE::kGamepad))));
                logger::debug("KeyCode for Gamepad block is {} \n", blockKeyController.GetPattern());
                break;
            }
        }
    }

    RE::BSEventNotifyControl HotkeyManager::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource)
    {
        using EventType = RE::INPUT_EVENT_TYPE;
        using Result = RE::BSEventNotifyControl;

        if (!a_event)
            return Result::kContinue;

        const auto ui = RE::UI::GetSingleton();
        if (ui->GameIsPaused()) {
            return Result::kContinue;
        }
        blockKey.Process(a_event);
        blockKeyMouse.Process(a_event);
        blockKeyController.Process(a_event);
        dualBlockKey.Process(a_event);

        return Result::kContinue;
    }
    HotkeyManager::HotkeyManager() {
        if (!dualBlockKey.SetPattern(Settings::dualblockKey))
            logger::error("Failed to set {} for block key", Settings::dualblockKey);
    }

    MenuManager* MenuManager::GetSingleton()
    {
        static MenuManager singleton;
        return &singleton;
    }

    void MenuManager::RegisterMenuEvents()
    {
        if (const auto scripts = RE::UI::GetSingleton()) {
            scripts->AddEventSink<RE::MenuOpenCloseEvent>(GetSingleton());
            logger::info("Registered {}"sv, typeid(RE::MenuOpenCloseEvent).name());
        }

    }

    void MenuManager::SetBlockKey()
    {
        logger::debug("Started Looking up the block key...");
        HotkeyManager* km = HotkeyManager::GetSingleton();
        km->SetBlockKey();
    }

    RE::BSEventNotifyControl MenuManager::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
    {
        using Result = RE::BSEventNotifyControl;

        if (!a_event) {
            return Result::kContinue;
        }
        if (a_event->menuName != RE::JournalMenu::MENU_NAME) {
            return Result::kContinue;
        }
        if (a_event->opening) {
            SetBlockKey();
        }
        

        return Result::kContinue;
    }
    MenuManager::MenuManager() {

    }
}


