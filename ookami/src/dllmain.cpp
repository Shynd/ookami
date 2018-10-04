#include <Windows.h>
#include <iostream>
#include <map>

#include <ookami/detours/detour_manager.hpp>
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

DWORD WINAPI OnDllAttach(LPVOID lpvBase)
{
  AllocConsole() && AttachConsole(GetCurrentProcessId());

  // Setup the detours
  DetourManager::Initialize();

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