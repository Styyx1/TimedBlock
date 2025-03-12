#pragma once

class OnHitManager : public RE::BSTEventSink<RE::TESHitEvent>
{
public:

    static OnHitManager* GetSingleton();

    RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) override;
    static void ProcessHitForParry(RE::Actor* target, RE::Actor* aggressor);
    static void Register();
    static bool PerkLockedStagger(RE::Actor* target, RE::BGSPerk* a_perk);

private:
    OnHitManager() = default;
    OnHitManager(const OnHitManager&) = delete;
    OnHitManager(OnHitManager&&) = delete;
    OnHitManager& operator=(const OnHitManager&) = delete;
    OnHitManager& operator=(OnHitManager&&) = delete;

};
