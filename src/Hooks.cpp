#include "Hooks.h"
#include "Settings.h"
#include "Utility.h"
namespace Hooks
{
    void Install()
    {
        SKSE::AllocTrampoline(14);
        CombatHit::Install();
    }

    void CombatHit::Install()
    {
        //SE: 140628C20 - 37673
        auto& trampoline = SKSE::GetTrampoline();
        REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(37673, 38627), REL::VariantOffset(0x3c0, 0x4A8, 0x3c0) };
        _originalCall = trampoline.write_call<5>(target.address(), &Hit);
    }

    void CombatHit::PreventDamage(RE::Actor* a_this, RE::HitData* a_hitData)
    {
        const Settings* settings = Settings::GetSingleton();
        float remaining = a_hitData->totalDamage;
        if (Utility::ActorHasActiveEffect(a_this, settings->mgef_parry_window) && !a_hitData->weapon->IsBow() && !a_hitData->weapon->IsCrossbow() && !a_hitData->attackDataSpell) {
            logger::debug("started hooked damage prevention for {}", a_this->GetName());            
            a_hitData->totalDamage = 0.0f;
            _originalCall(a_this, a_hitData);
        }
        else {
            _originalCall(a_this, a_hitData);
            a_hitData->totalDamage = remaining;
        }
    }

    void CombatHit::Hit(RE::Actor* a_this, RE::HitData* a_hitData)
    {
        logger::debug("{} was hit for {} damage", a_this->GetName(), a_hitData->totalDamage);
        PreventDamage(a_this, a_hitData);
        _originalCall(a_this, a_hitData);
    }

} // namespace Hooks
