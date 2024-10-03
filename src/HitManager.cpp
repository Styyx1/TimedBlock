#include "HitManager.h"
#include "Utility.h"
#include "Settings.h"
#include "Hooks.h"

#define continueEv return RE::BSEventNotifyControl::kContinue;

RE::BSEventNotifyControl OnHitManager::ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource)
{
    using Result = RE::BSEventNotifyControl;
    using HitFlag = RE::TESHitEvent::Flag;
    if (!a_event || !a_event->cause || !a_event->target || a_event->projectile) {
        continueEv
    }
    auto defender = a_event->target ? a_event->target->As<RE::Actor>() : nullptr;
    if (!defender) {
        continueEv
    }
    auto aggressor = a_event->cause ? a_event->cause->As<RE::Actor>() : nullptr;
    if (!aggressor) {
        continueEv
    }
    if (a_event->flags.any(HitFlag::kHitBlocked) && a_event->target && !a_event->projectile) {
        logger::debug("entered block event");
        auto attacking_weap = RE::TESForm::LookupByID<RE::TESObjectWEAP>(a_event->source);
        if (!defender || !attacking_weap || !defender->GetActorRuntimeData().currentProcess || !defender->GetActorRuntimeData().currentProcess->high
            || !attacking_weap->IsMelee() || !defender->Get3D())
        {
            logger::debug("block event, first continue");
            continueEv
        }

        if (!aggressor || !aggressor->GetActorRuntimeData().currentProcess || !aggressor->GetActorRuntimeData().currentProcess->high) {
            logger::debug("Attack Actor Not Found!");
            continueEv
        }
        auto data_aggressor = aggressor->GetActorRuntimeData().currentProcess->high->attackData;
        if (!data_aggressor) {
            logger::debug("Attacker Attack Data Not Found!");
            continueEv
        }
        //auto meleeweap = util->getWieldingWeapon(aggressor);
        auto leftHand  = defender->GetEquippedObject(true);
        auto rightHand = defender->GetEquippedObject(false);
//currently shields and weapons are handled the same, but i want to leave the separation for possible future changes
        if (leftHand && leftHand->IsArmor()) {
            logger::debug("left hand is shield");
            ProcessHitForParry(defender, aggressor);            
        }
        else if (rightHand && rightHand->IsWeapon()) {
            logger::debug("right hand is weapon");
            ProcessHitForParry(defender, aggressor);
        }
    }
    continueEv
}

void OnHitManager::ProcessHitForParry(RE::Actor* target, RE::Actor* aggressor)
{
    logger::debug("processHitEvent For Parry started");
    auto settings = Settings::GetSingleton();
    if (Utility::PlayerHasActiveMagicEffect(settings->mgef_parry_window)) {
        logger::debug("condition is true");
        logger::debug("range is {}",15);
        for (auto& actors : Utility::GetNearbyActors(target, settings->stagger_distance, false)) {
            if (actors != aggressor) {
                Utility::ApplySpell(target, actors, settings->spell_parry);                
                logger::debug("applied spell to {}", actors->GetName());
            }
        }
        Utility::ApplySpell(target, aggressor, settings->spell_parry);
    }
}

void OnHitManager::Register()
{
    RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
    eventHolder->AddEventSink(OnHitManager::GetSingleton());
}
