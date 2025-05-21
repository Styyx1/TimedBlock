#include "Hooks.h"
#include "Settings.h"
#include "Utility.h"
namespace Hooks
{
    void Install()
    {
        
        CombatHit::Install();
    }
    void CombatHit::Install()
    {
        //SE: 140628C20 - 37673
        auto& trampoline = SKSE::GetTrampoline();
        REL::Relocation<std::uintptr_t> target{ REL_ID(37673, 38627), OFFSET_3(0x3c0, 0x4A8, 0x3c0) };
        _originalCall = trampoline.write_call<5>(target.address(), &CHit);
    }
    void CombatHit::PreventDamage(RE::Actor* a_this, RE::HitData* a_hitData)
    {        
        if (a_hitData) {
            float remaining = 0.0f;
            remaining = a_hitData->totalDamage;
            if (Utility::ActorHasActiveEffect(a_this, Settings::mgef_parry_window)  && Utility::IsBlocking(a_this) && a_hitData->flags.any(RE::HitData::Flag::kBlocked)) {
                logger::debug("started hooked damage prevention for {} who got hit by: {}", a_this->GetName(), a_hitData->aggressor.get().get()->GetDisplayFullName());
                if (a_hitData->weapon){
                    if (!a_hitData->weapon->IsBow() || !a_hitData->weapon->IsCrossbow()) {
                        if (Settings::perk_locked_block) {
                            if (Settings::damage_prevent_perk != nullptr && a_this->HasPerk(Settings::damage_prevent_perk)) {
                                logger::debug("has perk");
                                a_hitData->totalDamage = 0.0f;
                                return;
                            }
                            else {   
                                logger::debug("perk lock bool is true, but doesn't have perk");
                                a_hitData->totalDamage = remaining;
                                return;
                            }
                        }
                        else {
                            logger::debug("perk lock is false");
                            a_hitData->totalDamage = 0.0f;
                            return;
                        }
                    }
                   
                }
                else {
                    if (Settings::perk_locked_block) {
                        if (Settings::damage_prevent_perk != nullptr && a_this->HasPerk(Settings::damage_prevent_perk)) {
                            logger::debug("has perk");
                            a_hitData->totalDamage = 0.0f;
                            return;
                        }
                        else {   
                            logger::debug("perk lock bool is true, but doesn't have perk");
                            a_hitData->totalDamage = remaining;
                            return;
                        }
                    }
                    else {
                        logger::debug("perk lock is false");
                        a_hitData->totalDamage = 0.0f;            
                        return;
                    }
                }                            
            }
            else {
                a_hitData->totalDamage = remaining;
                return;
            }
        }        
    }
    void CombatHit::CHit(RE::Actor* a_this, RE::HitData* a_hitData)
    {
        PreventDamage(a_this, a_hitData);
        _originalCall(a_this, a_hitData);
    }
} // namespace Hooks
