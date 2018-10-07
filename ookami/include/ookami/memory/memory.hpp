#pragma once

template <typename T>
T Read(int addr)
{
  return *(T*)addr;
}