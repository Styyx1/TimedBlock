#include "Hooks.h"
#include "Logging.h"
#include "Settings.h"
#include "Cache.h"
#include "HotkeyManager.h"
#include "HitManager.h"

void Listener(SKSE::MessagingInterface::Message* message) noexcept
{
    if (message->type == SKSE::MessagingInterface::kDataLoaded) {
        Settings::LoadSettings();
        Settings::GetSingleton()->LoadForms();
        Events::HotkeyManager::Register();
        Events::MenuManager::RegisterMenuEvents();
        OnHitManager::Register();
        Hooks::Install();
    }
    if (message->type == SKSE::MessagingInterface::kPostLoadGame) {
        Events::HotkeyManager::GetSingleton()->SetBlockKey();
    }
    if (message->type == SKSE::MessagingInterface::kNewGame) {
        Events::HotkeyManager::GetSingleton()->SetBlockKey();
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    InitLogging();

    const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
    const auto name{ plugin->GetName() };
    const auto version{ plugin->GetVersion() };

    logger::info("{} {} is loading...", name, version);

    Init(skse);
    Cache::CacheAddLibAddresses();
    if (const auto messaging{ SKSE::GetMessagingInterface() }; !messaging->RegisterListener(Listener)) {
        return false;
    }

    logger::info("{} has finished loading.", name);
    logger::info("");

    return true;
}
