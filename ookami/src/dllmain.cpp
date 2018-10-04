#include <Windows.h>
#include <iostream>
#include <map>

#include <ookami/memory/detour.hpp>
#include <ookami/offsets.hpp>

// TODO: MOVE TO ANOTHER FILE, this is just here for easy testing.

using FrameScript__ExecuteBuffer_Fn = void(__fastcall*)(const char*,
                                                        const char*);

void PrintChat(const char* string)
{
  char buffer[256];
  snprintf(buffer, sizeof(buffer),
           "DEFAULT_CHAT_FRAME:AddMessage('%s', 0.0, 1.0, 0.0)", string);

  auto const framescript__executebuffer_fn =
    reinterpret_cast<FrameScript__ExecuteBuffer_Fn>(0x00704CD0);

  (framescript__executebuffer_fn)(buffer, "ookami");
}


std::map<std::string, MemoryOperation*> memory_operations{};

int __fastcall SignalEventDetour(int a1)
{
  char buffer[256];
  snprintf(buffer, sizeof(buffer), "FrameScript::SignalEvent: [ID: 0x%x]", a1);
  PrintChat(buffer);

  // ---- Return to original function ----
  auto detour = memory_operations["FrameScript__SignalEvent"];
  detour->Restore();
  int result = ((int(__fastcall*)(int))detour->target)(a1);
  detour->Apply();
  return result;
}

DWORD WINAPI OnDllAttach(LPVOID lpvBase)
{
  AllocConsole() && AttachConsole(GetCurrentProcessId());

  memory_operations["FrameScript__SignalEvent"] =
    new Detour(0x00703E50, (int)SignalEventDetour);

  return TRUE;
}

BOOL WINAPI OnDllDetach() { return TRUE; }

BOOL APIENTRY DllMain(_In_ HINSTANCE hInstDll,
                      _In_ DWORD dwReason,
                      _In_opt_ LPVOID lpvReserved)
{
  switch (dwReason) {
  case DLL_PROCESS_ATTACH:
    DisableThreadLibraryCalls(hInstDll);
    CreateThread(nullptr, 0, OnDllAttach, hInstDll, 0, nullptr);
    return TRUE;
  case DLL_PROCESS_DETACH:
    if (lpvReserved == nullptr) {
      return OnDllDetach();
    }
    return TRUE;
  default:
    return TRUE;
  }
}