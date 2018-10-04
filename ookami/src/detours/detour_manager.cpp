#include <ookami/detours/detour_manager.hpp>

#include <ookami/memory/detour.hpp>
#include <ookami/offsets.hpp>

// Detour handlers
#include <ookami/detours/framescript.hpp>

#include <iostream>
#include <map>

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

void DetourManager::Initialize()
{
  dt_mem["FrameScript__SignalEvent"] =
    new Detour(FRAMESCRIPT__SIGNALEVENT, (int)SignalEventDetour);
}