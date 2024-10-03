#pragma once
#include "Cache.h"

class Utility : public Singleton<Utility>
{
public:

    static bool IsBlocking(RE::Actor* actor)
    {
        using func_t = decltype(&Utility::IsBlocking);
        REL::Relocation<func_t> func{ Cache::IsBlockingAddress };
        return func(actor);
    }

    inline static REL::Relocation<decltype(IsBlocking)> _IsBlocking;

    inline static bool ActorHasActiveEffect(RE::Actor* a_actor, RE::EffectSetting* a_effect)
    {
        auto activeEffects = a_actor->AsMagicTarget()->GetActiveEffectList();
        RE::EffectSetting* setting       = nullptr;
        for (auto& effect : *activeEffects) {
            setting = effect ? effect->GetBaseObject() : nullptr;
            if (setting) {
                if (setting == a_effect) {
                    return true;
                }
            }
        }
        return false;
    }

    inline static bool PlayerHasActiveMagicEffect(RE::EffectSetting* a_effect)
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        auto               activeEffects = player->AsMagicTarget()->GetActiveEffectList();
        RE::EffectSetting* setting       = nullptr;
        for (auto& effect : *activeEffects) {
            setting = effect ? effect->GetBaseObject() : nullptr;
            if (setting) {
                if (setting == a_effect) {
                    return true;
                }
            }
        }
        return false;
    }

    // Credit: D7ry for getWieldingWeapon in ValhallaCombat
    // https://github.com/D7ry/valhallaCombat/blob/48fb4c3b9bb6bbaa691ce41dbd33f096b74c07e3/src/include/Utils.cpp#L10
    inline static RE::TESObjectWEAP* getWieldingWeapon(RE::Actor* a_actor)
    {
        bool dual_wielding = false;
        auto weapon        = a_actor->GetAttackingWeapon();
        if (weapon) {
            dual_wielding = false;
            return weapon->object->As<RE::TESObjectWEAP>();
        }
        auto rhs = a_actor->GetEquippedObject(false);
        if (rhs && rhs->IsWeapon()) {
            dual_wielding = false;
            return rhs->As<RE::TESObjectWEAP>();
        }
        auto lhs = a_actor->GetEquippedObject(true);
        if (lhs && lhs->IsWeapon()) {
            dual_wielding = false;
            return lhs->As<RE::TESObjectWEAP>();
        }

        return nullptr;
    }

    inline static bool IsDualWielding(RE::Actor* a_actor)
    {
        auto weapon = a_actor->GetAttackingWeapon();
        auto rhs    = a_actor->GetEquippedObject(false);
        auto lhs    = a_actor->GetEquippedObject(true);
        if (weapon && rhs && lhs && lhs->IsWeapon() && rhs->IsWeapon()) {
            logger::debug("dual wielding is active");
            return true;
        }
        else
            return false;
    }

    inline static std::vector<RE::Actor*> GetNearbyActors(RE::TESObjectREFR* a_ref, float a_radius, bool a_ignorePlayer)
    {
        {
            std::vector<RE::Actor*> result;
            if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
                if (a_ignorePlayer && processLists->numberHighActors == 0) {
                    logger::debug("no process list");
                    return result;
                }

                const auto squaredRadius = a_radius * a_radius;
                const auto originPos     = a_ref->GetPosition();

                result.reserve(processLists->numberHighActors);

                const auto get_actor_within_radius = [&](RE::Actor* a_actor) {
                    if (a_actor && a_actor != a_ref && originPos.GetSquaredDistance(a_actor->GetPosition()) <= squaredRadius) {
                        result.emplace_back(a_actor);
                    }
                    };
                for (auto& actorHandle : processLists->highActorHandles) {
                    const auto actor = actorHandle.get();
                    get_actor_within_radius(actor.get());
                }

                if (!a_ignorePlayer) {
                    get_actor_within_radius(Cache::GetPlayerSingleton());
                }

                if (!result.empty()) {
                    logger::debug("vector is not empty");
                    return result;
                }
            }
            return result;
        }
    }

    static bool IsPermanent(RE::MagicItem* item)
    {
        switch (item->GetSpellType()) {
        case RE::MagicSystem::SpellType::kDisease:
        case RE::MagicSystem::SpellType::kAbility:
        case RE::MagicSystem::SpellType::kAddiction: {
            return true;
        }
        default: {
            return false;
        }
        }
    }

    inline static void ApplySpell(RE::Actor* caster, RE::Actor* target, RE::SpellItem* spell)
    {
        if (IsPermanent(spell)) {
            target->AddSpell(spell);
        }
        else {
            caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(spell, false, target, 1.0F, false, 0.0F, nullptr);
        }
    }
};
