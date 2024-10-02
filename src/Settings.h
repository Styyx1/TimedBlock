#pragma once

class Settings : public Singleton<Settings>
{
public:
    static void LoadSettings() noexcept;
    void LoadForms() noexcept;
    void LogForm(RE::TESForm* form_to_log);
    inline static bool debug_logging{};
    inline static std::string dualblockKey = "V";
    inline static double stagger_distance{ 25.0 };

    RE::BGSPerk* damage_prevent_perk;
    RE::SpellItem* spell_parry;
    RE::SpellItem* spell_parry_window;
    RE::SpellItem* spell_parry_buff;
    RE::EffectSetting* mgef_parry_window;
};
