#pragma once

namespace Hooks
{
    void Install();

    class CombatHit {
    public:
        static void Install();

    private:
        static void PreventDamage(RE::Actor* a_this, RE::HitData* a_hitData);
        static void CHit(RE::Actor* a_this, RE::HitData* a_hitData);
        static inline REL::Relocation<decltype(&CHit)> _originalCall;
    };
} // namespace Hooks
