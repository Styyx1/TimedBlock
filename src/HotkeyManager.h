#pragma once
#include "Settings.h"
#include <Xinput.h>
namespace Events {
    class HotkeyManager : public RE::BSTEventSink<RE::InputEvent*>
    {
    public:
        static HotkeyManager* GetSingleton();
        static void Register();
        static void TimedBlockKey(const hotkeys::KeyCombination* key);
        hotkeys::KeyCombination blockKey{ TimedBlockKey };
        hotkeys::KeyCombination blockKeyMouse{ TimedBlockKey };
        hotkeys::KeyCombination blockKeyController{ TimedBlockKey };
        hotkeys::KeyCombination dualBlockKey{ TimedBlockKey };
        void SetBlockKey();

    protected:
        RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;
    private:
        // Singleton stuff
        HotkeyManager();
        HotkeyManager(const HotkeyManager&) = delete;
        HotkeyManager(HotkeyManager&&) = delete;
        ~HotkeyManager() override = default;
        HotkeyManager& operator=(const HotkeyManager&) = delete;
        HotkeyManager& operator=(HotkeyManager&&) = delete;
    };
    class MenuManager : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
    {
        // Singleton stuff
        MenuManager();
        MenuManager(const MenuManager&) = delete;
        MenuManager(MenuManager&&) = delete;
        ~MenuManager() override = default;
        MenuManager& operator=(const MenuManager&) = delete;
        MenuManager& operator=(MenuManager&&) = delete;

    public:
        static MenuManager* GetSingleton();
        static void RegisterMenuEvents();
        static void SetBlockKey();

    protected:
        RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
    };
}

