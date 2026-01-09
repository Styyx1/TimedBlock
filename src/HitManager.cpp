#include "HitManager.h"
#include "Settings.h"

namespace Events {
    void TimedBlock::InstallHit()
    {
        if (RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton(); eventHolder) {
            eventHolder->AddEventSink(this);
            logger::info("Registered for <RE::TESHitEvent>");
        }        
    }
    RES TimedBlock::ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>*)
    {
        using HitFlag = RE::TESHitEvent::Flag;

        if (!a_event || !a_event->cause || !a_event->source || !a_event->target || a_event->projectile) {
            return RES::kContinue;
        }
        auto defender = a_event->target->As<RE::Actor>();
        if (!defender || !defender->GetHighProcess()) {
            return RES::kContinue;
        }
        auto aggressor = a_event->cause->As<RE::Actor>();
        if (!aggressor || !aggressor->GetHighProcess()) {
            return RES::kContinue;
        }
        bool hitBlocked = a_event->flags.any(HitFlag::kHitBlocked);
        if (!hitBlocked) {
            return RES::kContinue;
        }
        RE::TESObjectWEAP* attacking_weap = RE::TESForm::LookupByID<RE::TESObjectWEAP>(a_event->source);
        if (!attacking_weap || !attacking_weap->IsMelee()) {
            return RES::kContinue;            
        }

        const bool isH2H = attacking_weap->IsHandToHandMelee();
        const auto leftHand = defender->GetEquippedObject(true);
        const auto rightHand = defender->GetEquippedObject(false);

        if (leftHand && leftHand->IsArmor()) {
            ProcessParry(aggressor, defender, isH2H);
        }
        else if (rightHand && rightHand->IsWeapon()) {
            if (!Config::Options::only_shield_timed_block.GetValue()) {
                ProcessParry(aggressor, defender, isH2H);
            }
            else {
                if (!isH2H) {
                    defender->PlaceObjectAtMe(Config::Forms::timed_block_explosion, false);                    
                }
                IncreaseTBCounter();                   
            }
        }
        return RES::kContinue;
    }
    void TimedBlock::ProcessParry(RE::Actor* a_aggressor, RE::Actor* a_target, bool attackedByHandToHand)
    {
        if (!ActorUtil::IsEffectActive(a_target, Config::Forms::mgef_parry_window)) {
            return;
        }
        if(!attackedByHandToHand)
            a_target->PlaceObjectAtMe(Config::Forms::timed_block_explosion, false);

        MagicUtil::ApplySpell(a_target, a_target, Config::Forms::spell_parry_buff);
        IncreaseTBCounter();

        if (!CanStaggerWithPerkLock(a_target)) {
            return;
        }
        if(a_target->IsPlayerRef())
            SendTBModEvent(a_target, a_aggressor);

        for (auto& enemy : ActorUtil::GetNearbyActors(a_target, Config::Options::stagger_distance.GetValue(), false)) {
            if (enemy != a_aggressor && enemy != a_target) {
                if (enemy->Is3DLoaded() && !enemy->IsDead()) {
                    MagicUtil::ApplySpell(a_target, enemy, Config::Forms::spell_parry);
                }
            }
            MagicUtil::ApplySpell(a_target, a_aggressor, Config::Forms::spell_parry);
        }
         
    }
    bool TimedBlock::IsStaggerPerkLocked() const
    {
        if (Config::Options::perk_locked_stagger.GetValue()) {
            if (Config::Forms::stagger_perk) {
                return true;
            }
        }
        return false;
    }
    bool TimedBlock::CanStaggerWithPerkLock(RE::Actor* a_target) const
    {
        if (IsStaggerPerkLocked()) {
            return a_target->HasPerk(Config::Forms::stagger_perk);
        }
        return true;
    }
    void TimedBlock::IncreaseTBCounter()
    {
        if (Config::Forms::timed_block_counter_glob)
            MathUtil::AddWithCap(Config::Forms::timed_block_counter_glob->value, 1.f, 5000.f);
    }
    void TimedBlock::SendTBModEvent(RE::Actor* a_defender, RE::Actor* a_attacker)
    {
        const auto attackerID = a_attacker ? a_attacker->GetFormID() : 0x0;
        const auto level = a_attacker ? a_attacker->GetLevel() : uint16_t(0);
        const float level_arg = static_cast<float>(level);
        const SKSE::ModCallbackEvent modEvent{ .eventName = RE::BSFixedString("STBL_OnTimedBlockDefender"), .strArg = RE::BSFixedString(std::to_string(attackerID)), .numArg = level_arg, .sender = a_defender };
        const SKSE::ModCallbackEvent modEventATK{ .eventName = RE::BSFixedString("STBL_OnTimedBlockAttacker"), .strArg = RE::BSFixedString(), .numArg = level_arg, .sender = a_attacker };
        SKSE::GetModCallbackEventSource()->SendEvent(&modEvent);
        SKSE::GetModCallbackEventSource()->SendEvent(&modEventATK);
    }
}
