#pragma once
#include "Cache.h"

class Utility 
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
        auto activeEffects = a_actor->GetActiveEffectList();
        RE::EffectSetting* setting       = nullptr;
        if (!activeEffects->empty()) {
            for (RE::ActiveEffect* effect : *activeEffects) {
                if (effect; !effect->flags.any(RE::ActiveEffect::Flag::kInactive)) {
                    setting = effect ? effect->GetBaseObject() : nullptr;
                    if (setting) {
                        if (setting == a_effect) {
                            return true;
                        }
                    }
                }
            }
        } 
        return false;
    }

    inline static bool PlayerHasActiveMagicEffect(RE::EffectSetting* a_effect)
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        auto               activeEffects = player->GetActiveEffectList();
        RE::EffectSetting* setting       = nullptr;
        if (!activeEffects->empty()) {
            for (RE::ActiveEffect* effect : *activeEffects) {
                if (effect; !effect->flags.any(RE::ActiveEffect::Flag::kInactive)) {
                    setting = effect ? effect->GetBaseObject() : nullptr;
                    if (setting) {
                        if (setting == a_effect) {
                            return true;
                        }
                    }
                }
            }
        } 
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
