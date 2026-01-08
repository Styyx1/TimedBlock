#include "Hooks.h"
#include "Settings.h"
namespace Hooks
{
    void Install()
    {
        
        CombatHit::Install();
    }
    void CombatHit::Install()
    {
        auto& trampoline = SKSE::GetTrampoline();
        REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(37673, 38627), OFFSET_3(0x3c0, 0x4A8, 0x3c0) };
        _originalCall = trampoline.write_call<5>(target.address(), &CHit);
    }
    void CombatHit::PreventDamage(RE::Actor* a_this, RE::HitData* a_hitData)
    {        
        if (!a_this || !a_hitData)
            return;

        if (a_hitData->weapon) {
            if (a_hitData->weapon->IsBow() || a_hitData->weapon->IsCrossbow())
                return;
        }      

        if (!CanPreventDamageWithPerkLock(a_this))
            return;
        const bool got_blocked = a_hitData->flags.any(RE::HitData::Flag::kBlocked);
        if (!ActorUtil::IsEffectActive(a_this, Config::Forms::mgef_parry_window) || !a_this->IsBlocking() || !got_blocked)
            return;

        a_hitData->totalDamage = 0.0f;
        a_hitData->stagger = 0.0f;
        a_hitData->criticalDamageMult = 0.0f;
    }
    void CombatHit::CHit(RE::Actor* a_this, RE::HitData* a_hitData)
    {
        PreventDamage(a_this, a_hitData);
        _originalCall(a_this, a_hitData);
    }
    bool CombatHit::IsDamagePreventionPerkLocked()
    {
        if (Config::Options::perk_locked_block.GetValue()) {
            if (Config::Forms::damage_prevent_perk) {
                return true;
            }
        }
        return false;
    }
    bool CombatHit::CanPreventDamageWithPerkLock(RE::Actor* a_defender)
    {
        if (IsDamagePreventionPerkLocked()) {
            return a_defender->HasPerk(Config::Forms::damage_prevent_perk);
        }
        return true;
    }
}
