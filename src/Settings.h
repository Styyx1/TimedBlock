#pragma once

class Settings 
{
public:
    static void LoadSettings() noexcept;
    static void LoadForms() noexcept;
    static void LogForm(RE::TESForm* form_to_log);
    static RE::FormID ParseFormID(const std::string& str);
    inline static bool debug_logging{false};
    static inline bool perk_locked_block{false};
    static inline bool only_shield_tb{false};
    static inline bool perk_lock_stagger{ false }; 
    inline static std::string dualblockKey = "V";
    inline static float stagger_distance{ 128.0f };
    inline static std::string perkModName;

    static inline RE::FormID timed_block_perk_form_id;
    static inline RE::BGSPerk* damage_prevent_perk;
    static inline RE::SpellItem* spell_parry;
    static inline RE::SpellItem* spell_parry_window;
    static inline RE::SpellItem* spell_parry_buff;
    static inline RE::EffectSetting* mgef_parry_window;
    static inline RE::BGSExplosion* timed_block_explosion;

};
