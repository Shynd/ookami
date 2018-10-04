#include <iostream>

#include <ookami/functions/lua.hpp>
#include <ookami/offsets.hpp>

void Lua::DoString(const char* command)
{
  auto const framescript__executebuffer =
    reinterpret_cast<FrameScript__ExecuteBuffer_Fn>(FRAMESCRIPT__EXECUTEBUFFER);

  (framescript__executebuffer)(command, "ookami");
}

void Lua::PrintChat(const char* message)
{
  char buffer[256];
  snprintf(buffer, sizeof(buffer),
           "DEFAULT_CHAT_FRAME:AddMessage('%s', 0.0, 1.0, 0.0)", message);

  DoString(buffer);
}