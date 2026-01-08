#include "Hooks.h"
#include "Settings.h"
#include "Cache.h"
#include "HotkeyManager.h"
#include "HitManager.h"
#include "menu.h"


void InitializeLog()
{
    auto path = logger::log_directory();
    if (!path) {
        SKSE::stl::report_and_fail("Failed to find standard logging directory"sv);
    }

    *path /= "simple-timed-block.log"sv;
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

    auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

    log->set_level(spdlog::level::info);
    log->flush_on(spdlog::level::info);

    spdlog::set_default_logger(std::move(log));
    spdlog::set_pattern("[%H:%M:%S] [%l] %v"s);;
}

void Listener(SKSE::MessagingInterface::Message* message) noexcept
{
    if (message->type == SKSE::MessagingInterface::kDataLoaded) {

        SKSE::Translation::ParseTranslation("SimpleTimedBlock");
        Menu::RegisterTBMenu();
        Menu::TranslateMenu();
        Config::Forms::GetSingleton()->LoadForms();
        Events::InputEventListener::GetSingleton()->RegisterInput();
        Events::TimedBlock::GetSingleton()->InstallHit();
        Hooks::Install();
        Config::Options::perk_locked_block.SetValue(false);
        Config::Options::GetSingleton()->UpdateSettings(true);
        Menu::RegisterMenuCloseEvent();
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    Init(skse);
#ifndef SKYRIM_SUPPORT_AE
    InitializeLog();
#endif
    SKSE::AllocTrampoline(14);
    Cache::CacheAddLibAddresses();
    Config::Options::GetSingleton()->UpdateSettings(false);
    
    if (const auto messaging{ SKSE::GetMessagingInterface() }; !messaging->RegisterListener(Listener)) {
        return false;
    }
    return true;
}
