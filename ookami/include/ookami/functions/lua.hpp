#pragma once

using FrameScript__ExecuteBuffer_Fn = void(__fastcall*)(const char*,
                                                        const char*);

class Lua
{
public:
  static void DoString(const char* command);
  static void PrintChat(const char* message);
};