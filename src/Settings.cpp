#include "Settings.h"

#include "HotkeyManager.h"

bool ReadHotkey(const CSimpleIniA& ini, const char* name, std::string& pattern, hotkeys::KeyCombination& hotkey) {
    if (const auto rawPattern = ini.GetValue("", name)) {
        if (hotkey.SetPattern(rawPattern)) {
            pattern = rawPattern;
            return true;
        }
        logger::error("Failed to set '{}' as a hotkey for {}", pattern, name);
    }
    return false;
}
void Settings::LoadSettings() noexcept
{
    logger::info("Loading settings");
    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\SimpleTimedBlock.ini)");
    const auto manager = Events::HotkeyManager::GetSingleton();
    debug_logging = ini.GetBoolValue("Log", "Debug");
    ini::get_value(ini, stagger_distance, "", "fStaggerDistance");
    ReadHotkey(ini, "sBlockKey", dualblockKey, manager->blockKey);

    if (debug_logging) {
        spdlog::set_level(spdlog::level::debug);
        logger::debug("Debug logging enabled");
    }
    logger::info("Loaded settings");
    logger::info("");
}

void Settings::LogForm(RE::TESForm* form_to_log)
{
    logger::debug("Found {} it is {} with an ID of {}", RE::FormTypeToString(form_to_log->GetFormType()), form_to_log->GetName(), form_to_log->GetFormID());
}

void Settings::LoadForms() noexcept
{
    const char* mod = "SimpleTimedBlock.esp";
    const int parrySpellID = 0x803;
    const int parryWindow = 0x802;
    //const int parrySelfBuff = 0x808; not yet implemented in esp
    const int parryWindowEFFECT = 0x801;
    const int damagePreventPerk = 0x800;

    RE::TESDataHandler* const dh = RE::TESDataHandler::GetSingleton();
    spell_parry = dh->LookupForm<RE::SpellItem>(parrySpellID, mod);
    LogForm(spell_parry);
    spell_parry_window = dh->LookupForm<RE::SpellItem>(parryWindow, mod);
    LogForm(spell_parry_window);
    mgef_parry_window = dh->LookupForm<RE::EffectSetting>(parryWindowEFFECT, mod);
    LogForm(mgef_parry_window);
    damage_prevent_perk = dh->LookupForm<RE::BGSPerk>(damagePreventPerk, mod);
    LogForm(damage_prevent_perk);
}
