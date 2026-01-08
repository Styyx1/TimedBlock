#pragma once

#include "st-ui.h"
#include "mod-data.h"
#include "Settings.h"

namespace Menu {
	using ux = StyyxUtil::UIUtil;

	namespace Menu::State {
		inline bool setting_changed = false;

		inline void MarkChanged() {
			if(!setting_changed)
				setting_changed = true;
		}
		inline void MarkSaved() {
			if (setting_changed)
				setting_changed = false;
		};
	}

	namespace Titles {
		inline std::string MOD_TITLE = "Simple Timed Block";
		inline std::string SETTINGS_SEC = "Settings";
	}
	namespace Label {

		inline std::string system = FontAwesome::UnicodeToUtf8(0xf390) + " $System";
		inline std::string save_settings = FontAwesome::UnicodeToUtf8(0xf0c7) + " $Save Settings";
		inline std::string restore_defaults = FontAwesome::UnicodeToUtf8(0xf0e2) + " $Reset Settings";
	}
	inline bool setting_changed = false;

	void __stdcall MenuEventListener(SKSEMenuFramework::Model::EventType eventType);
	void RegisterMenuCloseEvent();
	void RegisterTBMenu();
	void RestoreFromSettings();
	void TranslateMenu();
	bool __stdcall OnInput(RE::InputEvent* event);
	void ResetDefaults();
	void RenderSystem();
	
	namespace Settings {

		inline bool capture_key_input = false;

		namespace Label {
			inline std::string perk_locked_block = "$TimedBlock_PreventDamage";
			inline std::string only_shield_timed_block = "$TimedBlock_WithShield";
			inline std::string perk_locked_stagger = "$TimedBlock_LockStagger";
			inline std::string stagger_distance = "$TimedBlock_StaggerDistance";
			inline std::string dual_block_key = "$TimedBlock_DualBlock";
		}

		namespace Tool {
			inline std::string perk_locked_block = "$TimedBlock_TooltipPreventDamage";
			inline std::string only_shield_timed_block = "$TimedBlock_TooltipWithShield";
			inline std::string perk_locked_stagger = "$TimedBlock_TooltipLockStagger";
			inline std::string stagger_distance = "$TimedBlock_TooltipStaggerDistance";
			inline std::string dual_block_key = "$TimedBlock_TooltipDualBlock";
		}

		struct Labels {
			const char* key;
			std::string* value;
		};

		inline Labels kLabels[] = {
			{ "$TimedBlock_PreventDamage", &Label::perk_locked_block },
			{ "$TimedBlock_WithShield", &Label::only_shield_timed_block },
			{ "$TimedBlock_LockStagger", &Label::perk_locked_stagger },
			{ "$TimedBlock_StaggerDistance", &Label::stagger_distance },
			{ "$TimedBlock_DualBlock", &Label::dual_block_key },

			{ "$TimedBlock_TooltipPreventDamage", &Tool::perk_locked_block },
			{ "$TimedBlock_TooltipWithShield", &Tool::only_shield_timed_block },
			{ "$TimedBlock_TooltipLockStagger", &Tool::perk_locked_stagger },
			{ "$TimedBlock_TooltipStaggerDistance", &Tool::stagger_distance },
			{ "$TimedBlock_TooltipDualBlock", &Tool::dual_block_key },
		};

		namespace Var {
			inline bool perk_locked_block = Config::Options::perk_locked_block.GetValue();
			inline bool only_shield_timed_block = Config::Options::only_shield_timed_block.GetValue();
			inline bool perk_locked_stagger = Config::Options::perk_locked_stagger.GetValue();
			inline uint32_t dual_block_key = Config::Options::dual_block_key.GetValue();
			inline double stagger_distance = Config::Options::stagger_distance.GetValue();
		}
		void RenderKeySelector();
		void RenderOptions();
		void __stdcall RenderSettings();





	}


}