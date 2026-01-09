#pragma once
#include "mod-data.h"

namespace Config {
    using namespace MOD;

    struct Options : REX::Singleton<Options> {
        static inline REX::TOML::Bool perk_locked_block{ SEC_SETTINGS, "bPerkLockTimedBlocking", false };
        static inline REX::TOML::Bool only_shield_timed_block{ SEC_SETTINGS, "bOnlyShieldTimedBlock", false };
        static inline REX::TOML::Bool perk_locked_stagger{ SEC_SETTINGS, "bPerkLockStagger", false };
        static inline REX::TOML::U32 dual_block_key{ SEC_SETTINGS, "uDualBlockKey", uint32_t(47) };
        static inline REX::TOML::F64 stagger_distance{ SEC_SETTINGS, "fStaggerDistance", 128.0 };
        static inline REX::TOML::Str damage_prevent_perk_form{ SEC_SETTINGS, "sDamagePreventPerk", std::string("Skyrim.esm|0x58f68") };
        static inline REX::TOML::Str stagger_perk_form{ SEC_SETTINGS, "sStaggerPerk", std::string("Skyrim.esm|0x58f68") };

        void UpdateSettings(bool a_save) {
            const auto toml = REX::TOML::SettingStore::GetSingleton();
            toml->Init(TOML_DEF.data(), TOML_CUS.data());
            if (!a_save)
                toml->Load();
            else
                toml->Save();
        }
    };

    struct Forms : REX::Singleton<Forms> {

        static inline RE::BGSPerk* damage_prevent_perk;
        static inline RE::SpellItem* spell_parry;
        static inline RE::SpellItem* spell_parry_window;
        static inline RE::SpellItem* spell_parry_buff;
        static inline RE::EffectSetting* mgef_parry_window;
        static inline RE::BGSExplosion* timed_block_explosion;
        static inline RE::BGSPerk* stagger_perk;
        static inline RE::TESGlobal* timed_block_counter_glob;        

        RE::TESForm* LoadPerkFromConfigWithFallback(const std::string& form_string, RE::FormID a_fallbackID, const std::string& a_fallbackModName = MOD::MOD_FILE.data()) {
            if (!form_string.empty()) {
                const auto form = FormUtil::GetFormFromString(form_string);
                if (form) {
                    return form;
                }
                else {
                    auto dh = RE::TESDataHandler::GetSingleton();
                    return dh->LookupForm(a_fallbackID, a_fallbackModName);
                }                    
            }
        };

        void LoadForms() {
            auto dh = RE::TESDataHandler::GetSingleton();

            if (!MiscUtil::IsModLoaded(MOD_FILE)) {
                const auto fail_msg = std::format("Can not find forms from {}! Make sure the mod is enabled. Forcing the game to load regardless will result in crashes!", MOD_FILE);
                SKSE::stl::report_and_fail(fail_msg);
            }

            spell_parry = dh->LookupForm<RE::SpellItem>(ID_PARRY_SPELL, MOD_FILE);
            spell_parry_window = dh->LookupForm<RE::SpellItem>(ID_PARRY_WINDOW_SPELL, MOD_FILE);
            mgef_parry_window = dh->LookupForm<RE::EffectSetting>(ID_PARRY_WINDOW_EFFECT, MOD_FILE);
            spell_parry_buff = dh->LookupForm<RE::SpellItem>(ID_PARRY_SELF_BUFF, MOD_FILE);
            timed_block_explosion = dh->LookupForm<RE::BGSExplosion>(ID_BLOCK_EXPLOSION, MOD_FILE);
            timed_block_counter_glob = dh->LookupForm<RE::TESGlobal>(ID_TIMEDBLOCK_COUNTER_GLOB, MOD_FILE);

            auto prevent_form = LoadPerkFromConfigWithFallback(Options::damage_prevent_perk_form.GetValue(), ID_DAMAGE_PREVENT_PERK_FALLBACK);
            damage_prevent_perk = prevent_form ? prevent_form->As<RE::BGSPerk>() : nullptr;
            if (!damage_prevent_perk) {
                logger::warn("Can not find damage prevent perk or fallback");
            }
            else {
                logger::debug("Perk loaded is: {}", damage_prevent_perk->GetName());
            }

            auto stagger_form = LoadPerkFromConfigWithFallback(Options::stagger_perk_form.GetValue(), ID_STAGGER_PERK_FALLBACK);
            stagger_perk = stagger_form ? stagger_form->As<RE::BGSPerk>() : nullptr;
            if (!stagger_perk) {
                logger::warn("Can not find stagger perk or fallback");
            }
            else {
                logger::debug("Perk loaded is: {}", stagger_perk->GetName());
            }
        }

    };

}