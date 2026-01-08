#pragma once

namespace MOD {
	inline constexpr std::string_view TOML_DEF = "Data/SKSE/Plugins/timed-block.toml";
	inline constexpr std::string_view TOML_CUS = "Data/SKSE/Plugins/timed-block_custom.toml";
	inline constexpr std::string_view TRANSLATION_FILE_NAME = "simple-timed-block";
	inline constexpr std::string_view SEC_SETTINGS = "Settings";

	inline constexpr std::string_view MOD_NAME = "Simple Timed Block";
	inline constexpr std::string_view MOD_FILE = "SimpleTimedBlock.esp";
	inline constexpr RE::FormID ID_PARRY_SPELL = 0x803;
	inline constexpr RE::FormID ID_PARRY_WINDOW_SPELL = 0x802;
	inline constexpr RE::FormID ID_PARRY_SELF_BUFF = 0x80B;
	inline constexpr RE::FormID ID_PARRY_WINDOW_EFFECT = 0x801;
	inline constexpr RE::FormID ID_DAMAGE_PREVENT_PERK_FALLBACK = 0x800;
	inline constexpr RE::FormID ID_STAGGER_PERK_FALLBACK = 0x80D;
	inline constexpr RE::FormID ID_BLOCK_EXPLOSION = 0x805;
	inline constexpr RE::FormID ID_TIMEDBLOCK_COUNTER_GLOB = 0x80E;

}