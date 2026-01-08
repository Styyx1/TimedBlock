#pragma once

namespace Events {

    using RES = RE::BSEventNotifyControl;

    struct InputEventListener : REX::Singleton<InputEventListener>, RE::BSTEventSink<RE::InputEvent*> {

        void RegisterInput();
    private:
        RES ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) override;
        bool ProcessTBButton(RE::ButtonEvent* event) const;
    };
}
