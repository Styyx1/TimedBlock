#include "menu.h"

template <class T>
bool SettingSlider(const char* label, T& slider_var, const T min, const T max, const char* fmt,
    REX::TOML::Setting<T>& setting, const char* help)
{
    ImGuiMCP::SetNextItemWidth(200.f);
    bool changed = ImGuiMCP::SliderScalar(label, ImGuiMCP::ImGuiDataType_::ImGuiDataType_Float, &slider_var,
        &min, &max, fmt);
    if (changed)
    {
        setting.SetValue(slider_var);
        Menu::Menu::State::MarkChanged();
    }
    ImGuiMCP::SameLine();
    StyyxUtil::UIUtil::HelpMarker(help);
    return changed;
}

template <class T>
bool SettingSliderD(const char* label, T& slider_var, const T min, const T max, const char* fmt,
    REX::TOML::Setting<T>& setting, const char* help)
{
    ImGuiMCP::SetNextItemWidth(200.f);
    bool changed = ImGuiMCP::SliderScalar(label, ImGuiMCP::ImGuiDataType_::ImGuiDataType_Double, &slider_var,
        &min, &max, fmt);
    if (changed)
    {
        setting.SetValue(slider_var);
        Menu::Menu::State::MarkChanged();
    }
    ImGuiMCP::SameLine();
    StyyxUtil::UIUtil::HelpMarker(help);
    return changed;
}


template <class T>
bool SettingSliderINT(const char* label, T& slider_var, const T min, const T max, const char* fmt,
    REX::TOML::Setting<T>& setting, const char* help)
{

    ImGuiMCP::SetNextItemWidth(200.f);
    bool changed =
        ImGuiMCP::SliderScalar(label, ImGuiMCP::ImGuiDataType_::ImGuiDataType_S32, &slider_var, &min, &max, fmt);
    if (changed)
    {
        setting.SetValue(slider_var);
        Menu::Menu::State::MarkChanged();
    }
    ImGuiMCP::SameLine();
    StyyxUtil::UIUtil::HelpMarker(help);
    
    return changed;
}

inline bool SettingCheckbox(const char* label, bool& value_var, REX::TOML::Setting<bool>& setting, const char* help)
{
    bool changed = ImGuiMCP::Checkbox(label, &value_var);
    if (changed)
    {
        setting.SetValue(value_var);
        Menu::Menu::State::MarkChanged();
    }
    ImGuiMCP::SameLine();
    StyyxUtil::UIUtil::HelpMarker(help);
    return changed;
}

namespace Menu {
	void __stdcall MenuEventListener(SKSEMenuFramework::Model::EventType eventType)
	{
		if (eventType == SKSEMenuFramework::Model::EventType::kCloseMenu) {
			if (Menu::State::setting_changed) {
				Menu::State::MarkSaved();
				Config::Options::GetSingleton()->UpdateSettings(true);
			}
		}
	}
    SKSEMenuFramework::Model::Event* event;
	void RegisterMenuCloseEvent()
	{
        event = new SKSEMenuFramework::Model::Event(MenuEventListener);
        if (event) {
            logger::info("Registered skse menu open/close event");
        }
	}

    void RegisterTBMenu()
    {
        
        if (!SKSEMenuFramework::IsInstalled())
        {
            return;
        }
        
        SKSEMenuFramework::SetSection(Titles::MOD_TITLE);
        SKSEMenuFramework::AddSectionItem(Titles::SETTINGS_SEC, Settings::RenderSettings);
        RegisterMenuCloseEvent();
        SKSEMenuFramework::AddInputEvent(OnInput);
        RestoreFromSettings();
    }
    void RestoreFromSettings()
    {
        using opt = Config::Options;
        using namespace Settings::Var;
        perk_locked_block = Config::Options::perk_locked_block.GetValue();
        only_shield_timed_block = Config::Options::only_shield_timed_block.GetValue();
        perk_locked_stagger = Config::Options::perk_locked_stagger.GetValue();
        dual_block_key = Config::Options::dual_block_key.GetValue();
        stagger_distance = Config::Options::stagger_distance.GetValue();

    }
    void TranslateMenu()
    {

        std::string system;
        SKSE::Translation::Translate("$TimedBlock_System", system);
        Label::system = FontAwesome::UnicodeToUtf8(0xf390) + " " + system;
        std::string save_settings;
        SKSE::Translation::Translate("$TimedBlock_SaveSettings", save_settings);
        Label::save_settings = FontAwesome::UnicodeToUtf8(0xf0c7) + " " + save_settings;
        std::string restore_settings;
        SKSE::Translation::Translate("$TimedBlock_RestoreDefaults", restore_settings);
        Label::restore_defaults = FontAwesome::UnicodeToUtf8(0xf0e2) + " " + restore_settings;

        for (auto& entry : Settings::kLabels) {
            std::string translated;
            if (SKSE::Translation::Translate(entry.key, translated)) {
                *entry.value = translated;
            }
        }
        logger::info("Completed translation for menu");

    }
    bool __stdcall OnInput(RE::InputEvent* event)
    {
        bool blockThisUserInput = false;

        if (event->device == RE::INPUT_DEVICE::kKeyboard) {
            if (auto button = event->AsButtonEvent()) {
                if (Settings::capture_key_input) {
                    if (button->IsDown()) {
                        Settings::Var::dual_block_key = button->GetIDCode();
                    }
                }
            }
            return blockThisUserInput;
        }
        if (event->device == RE::INPUT_DEVICE::kGamepad) {
            if (auto button = event->AsButtonEvent()) {
                if (Settings::capture_key_input) {
                    if (button->IsDown()) {
                        Settings::Var::dual_block_key = SKSE::InputMap::GamepadMaskToKeycode(button->GetIDCode());
                    }
                }
            }
            return blockThisUserInput;
        }
        if (event->device == RE::INPUT_DEVICE::kMouse) {
            if (auto button = event->AsButtonEvent()) {
                if (Settings::capture_key_input) {
                    if (button->IsDown()) {
                        Settings::Var::dual_block_key = button->GetIDCode() + SKSE::InputMap::kMacro_MouseButtonOffset;
                    }
                }
            }
        }


        return blockThisUserInput;
    }
    void Settings::RenderKeySelector()
    {
        std::string key_name = clib_util::hotkeys::details::GetNameByKey(Config::Options::dual_block_key.GetValue()).data();

        //not really needed but looks better in the menu
        std::transform(key_name.begin(), key_name.end(), key_name.begin(), ::toupper);

        ImGuiMCP::Text(std::format("{}: {}",Label::dual_block_key, key_name).c_str());
        ImGuiMCP::SameLine();

        if (!capture_key_input) {
            if (ImGuiMCP::Button("Rebind")) {
                capture_key_input = true;
                Var::dual_block_key = 0;
            }
            ImGuiMCP::SameLine();
            ux::HelpMarker(Tool::dual_block_key.c_str());
        }
        else {
            ImGuiMCP::Text("Press any key");
            ImGuiMCP::SameLine();
            if (ImGuiMCP::Button("Cancel")) {
                capture_key_input = false;
                Var::dual_block_key = Config::Options::dual_block_key.GetValue();
            }
        }
        if (Var::dual_block_key != 0)
        {
            Config::Options::dual_block_key.SetValue(Var::dual_block_key);
            capture_key_input = false;
        }
    }
    void ResetDefaults()
    {
        Menu::State::MarkChanged();
        using opt = Config::Options;
        using namespace Settings::Var;

        perk_locked_block = false;
        only_shield_timed_block = false;
        perk_locked_stagger = false;
        stagger_distance = 128.0;
        dual_block_key = 47;

        opt::perk_locked_block.SetValue(perk_locked_block);
        opt::only_shield_timed_block.SetValue(only_shield_timed_block);
        opt::perk_locked_stagger.SetValue(perk_locked_stagger);
        opt::stagger_distance.SetValue(stagger_distance);
        opt::dual_block_key.SetValue(dual_block_key);
        
        opt::GetSingleton()->UpdateSettings(true);
    }
    void RenderSystem()
    {
        ImGuiMCP::NewLine();
        ImGuiMCP::SeparatorText(Label::system.c_str());

        if (ImGuiMCP::Button(Label::save_settings.c_str()))
        {
            Config::Options::GetSingleton()->UpdateSettings(true);
        }

        ImGuiMCP::SameLine();
        if (ImGuiMCP::Button(Label::restore_defaults.c_str()))
        {
            ResetDefaults();
        }
    }
}

void Menu::Settings::RenderOptions()
{
    namespace mc = ImGuiMCP;
    using namespace Settings::Var;
    using opt = Config::Options;

    SettingCheckbox(Label::perk_locked_block.c_str(), perk_locked_block, opt::perk_locked_block, Tool::perk_locked_block.c_str());
    mc::SameLine();
    SettingCheckbox(Label::only_shield_timed_block.c_str(), only_shield_timed_block, opt::only_shield_timed_block, Tool::only_shield_timed_block.c_str());
    mc::SameLine();
    SettingCheckbox(Label::perk_locked_stagger.c_str(), perk_locked_stagger, opt::perk_locked_stagger, Tool::perk_locked_stagger.c_str());
    SettingSliderD(Label::stagger_distance.c_str(), stagger_distance, 10.0, 512.0, "%.1f", opt::stagger_distance, Tool::stagger_distance.c_str());
}

void __stdcall Menu::Settings::RenderSettings()
{
    FontAwesome::PushSolid();
    RenderOptions();
    ImGuiMCP::NewLine();
    RenderKeySelector();
    ImGuiMCP::NewLine();
    RenderSystem();
    FontAwesome::Pop();
}
