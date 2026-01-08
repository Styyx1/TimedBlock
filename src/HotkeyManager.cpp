#include "HotkeyManager.h"
#include "Settings.h"
#include "Cache.h"

namespace Events {

    void InputEventListener::RegisterInput()
    {
        if (const auto scripts = RE::BSInputDeviceManager::GetSingleton()) {
            scripts->AddEventSink<RE::InputEvent*>(this);
            logger::info("Registered for {}", typeid(RE::InputEvent).name());
        }
    }

    RES InputEventListener::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*)
    {
        if (!a_event)
            return RES::kContinue;

        for (RE::InputEvent* evnt = *a_event; evnt; evnt = evnt->next) {

            if (auto button_event = evnt->AsButtonEvent()) {
                auto lAtk = button_event->userEvent == RE::UserEvents::GetSingleton()->leftAttack;
                if (lAtk) {
                    if (ProcessTBButton(button_event)) {
                        RES::kContinue;
                    }
                }

                if (button_event->device == RE::INPUT_DEVICE::kKeyboard) {
                    if (button_event->GetIDCode() == Config::Options::dual_block_key.GetValue()) {
                        if (ProcessTBButton(button_event)) {
                            RES::kContinue;
                        }
                    }
                }

                if (button_event->device == RE::INPUT_DEVICE::kGamepad) {
                    if (SKSE::InputMap::GamepadMaskToKeycode(button_event->GetIDCode() == Config::Options::dual_block_key.GetValue())) {
                        if (ProcessTBButton(button_event)) {
                            RES::kContinue;
                        }
                    }
                }

                if (button_event->device == RE::INPUT_DEVICE::kMouse) {
                    if (button_event->GetIDCode() + SKSE::InputMap::kMacro_MouseButtonOffset == Config::Options::dual_block_key.GetValue()) {
                        if (ProcessTBButton(button_event)) {
                            RES::kContinue;
                        }
                    }
                }
            }
        }
        return RES::kContinue;
    }
    bool InputEventListener::ProcessTBButton(RE::ButtonEvent* event) const
    {
        if (event->IsDown()) {
            const auto player = Cache::GetPlayerSingleton();
            if (!ActorUtil::IsEffectActive(player, Config::Forms::mgef_parry_window)) {
                MagicUtil::ApplySpell(player, player, Config::Forms::spell_parry_window);                
            }
            return true;
        }
        return false;
    }
}

    

