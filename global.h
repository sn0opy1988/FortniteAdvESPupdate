#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <Psapi.h>

#include "SDK.hpp"

namespace Global
{
    DWORD_PTR BaseAddress;
    MODULEINFO info;
    SDK::UWorld** m_UWorld;
    SDK::ULevel* m_PersistentLevel;
    SDK::ULocalPlayer* m_LocalPlayer;
    SDK::TArray<SDK::AActor*>* m_Actors;
}

#endif
