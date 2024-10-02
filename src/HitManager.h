#pragma once

class OnHitManager : public EventSingleton<OnHitManager, RE::TESHitEvent>
{
public:
    RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) override;
    static void ProcessHitForParry(RE::Actor* target, RE::Actor* aggressor);
    static void Register();

};
