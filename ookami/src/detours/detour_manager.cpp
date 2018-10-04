#include <ookami/detours/detour_manager.hpp>

#include <ookami/memory/detour.hpp>
#include <ookami/offsets.hpp>

// Detour handlers
#include <ookami/detours/framescript.hpp>

#include <iostream>
#include <map>

#include <ookami/functions/lua.hpp>

std::map<std::string, MemoryOperation*> dt_mem{};

int __fastcall SignalEventDetour(int a1)
{
  // Pass execution flow to the handler.
  FrameScript::SignalEvent(a1);

  // ---- return to original function ----
  auto detour = dt_mem["FrameScript__SignalEvent"];
  detour->Restore();
  auto result = ((int(__fastcall*)(int))detour->target)(a1);
  detour->Apply();
  return result;
}

int __fastcall WardenDataDetour(int a1, int a2, int a3, int a4)
{
  char buffer[256];
  snprintf(buffer, sizeof(buffer),
           "[SMSG_WARDEN_DATA] [unk0: %d] [unk1: %d] [unk2: %d] [unk3: %d]", a1,
           a2, a3, a4);
  Lua::PrintChat(buffer);

  // ---- return to original function ----
  auto detour = dt_mem["SMSG_WARDEN_DATA"];
  detour->Restore();
  auto result =
    ((int(__fastcall*)(int, int, int, int))detour->target)(a1, a2, a3, a4);
  detour->Apply();
  return result;
}

void DetourManager::Initialize()
{
  dt_mem["FrameScript__SignalEvent"] =
    new Detour(FRAMESCRIPT__SIGNALEVENT, (int)SignalEventDetour);

  dt_mem["SMSG_WARDEN_DATA"] =
    new Detour(SMSG_WARDEN_DATA, (int)WardenDataDetour);
}