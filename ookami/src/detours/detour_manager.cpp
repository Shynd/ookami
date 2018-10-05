#include <ookami/detours/detour_manager.hpp>

#include <ookami/memory/detour.hpp>
#include <ookami/offsets.hpp>

// Detour handlers
#include <ookami/detours/framescript.hpp>

#include <iostream>
#include <map>
#include <stdio.h>

#include <ookami/functions/lua.hpp>
#include <ookami/objmgr/objmgr.hpp>

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
  printf("[SMSG_WARDEN_DATA] [unk0: %d] [unk1: %d] [unk2: %d] [unk3: %d]\n", a1,
         a2, a3, a4);

  // ---- return to original function ----
  auto detour = dt_mem["SMSG_WARDEN_DATA"];
  detour->Restore();
  auto result =
    ((int(__fastcall*)(int, int, int, int))detour->target)(a1, a2, a3, a4);
  detour->Apply();
  return result;
}

int __fastcall EndSceneDetour(int this_ptr)
{
  ObjMgr::EnumVisibleObjects();

  // ---- return to original function ----
  auto detour = dt_mem["EndScene"];
  detour->Restore();
  auto result = ((int(__thiscall*)(int))detour->target)(this_ptr);
  detour->Apply();
  return result;
}

int __fastcall SendChatMessageDetour(int a1, int a2)
{
  char buffer[256];
  snprintf(buffer, sizeof(buffer), "Script::SendChatMessage: unk0: %d unk2: %d",
           a1, a2);
  Lua::PrintChat(buffer);

  // ---- return to original function ----
  auto detour = dt_mem["Script__SendChatMessage"];
  detour->Restore();
  auto result = ((int(__fastcall*)(int, int))detour->target)(a1, a2);
  detour->Apply();
  return result;
}

void DetourManager::Initialize()
{
  dt_mem["FrameScript__SignalEvent"] =
    new Detour(FRAMESCRIPT__SIGNALEVENT, (int)SignalEventDetour);

  dt_mem["SMSG_WARDEN_DATA"] =
    new Detour(SMSG_WARDEN_DATA, (int)WardenDataDetour);

  dt_mem["EndScene"] = new Detour(0x005A17A0, (int)EndSceneDetour);

  dt_mem["Script__SendChatMessage"] =
    new Detour(0x0049F1E0, (int)SendChatMessageDetour);
}