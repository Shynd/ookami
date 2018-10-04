#include <iostream>

#include <ookami/detours/framescript.hpp>
#include <ookami/functions/lua.hpp>

void FrameScript::SignalEvent(int event_id)
{
  char buffer[256];
  snprintf(buffer, sizeof(buffer), "FrameSript::SignalEvent: [0x%x]", event_id);
  Lua::PrintChat(buffer);
}