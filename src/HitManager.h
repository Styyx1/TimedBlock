#pragma once

namespace Events {

    using RES = RE::BSEventNotifyControl;

    struct TimedBlock : REX::Singleton<TimedBlock>, RE::BSTEventSink<RE::TESHitEvent> {
        void InstallHit();

    private: 
        RES ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>*) override;
        void ProcessParry(RE::Actor* a_aggressor, RE::Actor* a_target, bool attackedByHandToHand);
        bool IsStaggerPerkLocked() const;
        bool CanStaggerWithPerkLock(RE::Actor* a_target) const;
        void IncreaseTBCounter();
    };
}
