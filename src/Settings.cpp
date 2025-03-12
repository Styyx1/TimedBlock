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
    ini.LoadFile(R"(.\Data\SKSE\Plugins\simple-timed-block.ini)");
    const auto manager = Events::HotkeyManager::GetSingleton();
    debug_logging = ini.GetBoolValue("Log", "Debug");
    ini::get_value(ini, stagger_distance, "", "fStaggerDistance");
    perk_locked_block = ini.GetBoolValue("", "bPerkLockedBlock", false);
    only_shield_tb = ini.GetBoolValue("", "bOnlyWithShield", false);
    perk_lock_stagger = ini.GetBoolValue("", "bPerkLockedStagger", false);
    //ini::get_value(ini, perk_locked_block, "", "bPerkLockedBlock");
    ReadHotkey(ini, "sBlockKey", dualblockKey, manager->blockKey);
    std::string block_perk_id;
    ini::get_value(ini, block_perk_id, "Forms", "TimedBlockPerk");
    std::string perk_fileName;
    ini::get_value(ini, perk_fileName, "Forms", "PerkModName");

    if (debug_logging) {
        spdlog::set_level(spdlog::level::debug);
        logger::debug("Debug logging enabled");
    }

    if (!perk_fileName.empty()) {
        perkModName = perk_fileName;
        logger::debug("perk mod name is {}", perkModName);
        // Form ID from INI
        if (!block_perk_id.empty()) {
            timed_block_perk_form_id = ParseFormID(block_perk_id);
            logger::debug("found Block Perk FormID");
        }
    }

    logger::info("Loaded settings");
    logger::info("");
}

void Settings::LogForm(RE::TESForm* form_to_log)
{
    logger::debug("Found {} it is {} with an ID of {}", RE::FormTypeToString(form_to_log->GetFormType()), form_to_log->GetName(), form_to_log->GetFormID());
}

RE::FormID Settings::ParseFormID(const std::string& str)
{
    RE::FormID         result;
    std::istringstream ss{ str };
    ss >> std::hex >> result;
    return result;
}

void Settings::LoadForms() noexcept
{
    const char* mod = "SimpleTimedBlock.esp";
    const int parrySpellID = 0x803;
    const int parryWindow = 0x802;
    //const int parrySelfBuff = 0x808; not yet implemented in esp
    const int parryWindowEFFECT = 0x801;
    const int damagePreventPerk = 0x800;
    const int tb_explosionID = 0x805;
    RE::TESDataHandler* const dh = RE::TESDataHandler::GetSingleton();

    if (timed_block_perk_form_id) {
        damage_prevent_perk = skyrim_cast<RE::BGSPerk*>(dh->LookupForm(timed_block_perk_form_id, perkModName));
        LogForm(damage_prevent_perk);
    }
    
    spell_parry = dh->LookupForm<RE::SpellItem>(parrySpellID, mod);
    LogForm(spell_parry);
    spell_parry_window = dh->LookupForm<RE::SpellItem>(parryWindow, mod);
    LogForm(spell_parry_window);
    mgef_parry_window = dh->LookupForm<RE::EffectSetting>(parryWindowEFFECT, mod);
    LogForm(mgef_parry_window);
    timed_block_explosion = dh->LookupForm<RE::BGSExplosion>(tb_explosionID, mod);
    LogForm(timed_block_explosion);
    //damage_prevent_perk = dh->LookupForm<RE::BGSPerk>(damagePreventPerk, mod);
    
}
