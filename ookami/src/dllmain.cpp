#include <Windows.h>
#include <iostream>
#include <map>

#include <ookami/detours/detour_manager.hpp>

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