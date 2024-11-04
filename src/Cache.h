#pragma once

namespace Cache
{
    inline uintptr_t IsBlockingAddress;
    inline uintptr_t PlayerSingletonAddress;

    inline void CacheAddLibAddresses()
    {
        // 1.6 = 37952
        // 1.5.97 = 36927
        IsBlockingAddress = REL_ID(36927, 37952).address();

        // 1.6 = 403521
        // 1.5 = 517014
        PlayerSingletonAddress = REL_ID(517014, 403521).address();
    }

    inline RE::PlayerCharacter* GetPlayerSingleton()
    {
        REL::Relocation<RE::NiPointer<RE::PlayerCharacter>*> singleton{ PlayerSingletonAddress };
        return singleton->get();
    }

} // namespace Cache
